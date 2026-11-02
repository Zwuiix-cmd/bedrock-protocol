<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator\parser;

use PhpParser\Node;
use PhpParser\Node\Expr;
use PhpParser\NodeFinder;
use RuntimeException;

trait DeclarationParsingMethods
{
    private function phpFiles(string $directory): array
    {
        if (!is_dir($directory)) {
            throw new RuntimeException('Source does not contain src/: ' . $directory);
        }
        $files = [];
        $iterator = new \RecursiveIteratorIterator(
            new \RecursiveDirectoryIterator($directory, \FilesystemIterator::SKIP_DOTS),
        );
        foreach ($iterator as $file) {
            if ($file->isFile() && strtolower($file->getExtension()) === 'php') {
                $files[] = $file->getPathname();
            }
        }
        sort($files);
        return $files;
    }

    private function collectDeclarations(array $nodes, string $sourceRoot, string $file, string $namespace = ''): void
    {
        foreach ($nodes as $node) {
            if ($node instanceof Node\Stmt\Namespace_) {
                $this->collectDeclarations($node->stmts, $sourceRoot, $file, $node->name?->toString() ?? '');
                continue;
            }
            if ($node instanceof Node\Stmt\Class_ || $node instanceof Node\Stmt\Interface_ || $node instanceof Node\Stmt\Enum_ || $node instanceof Node\Stmt\Trait_) {
                $declaration = $this->declaration($node, $sourceRoot, $file, $namespace);
                if ($declaration !== null) {
                    $this->declarations[$declaration['fqcn']] = $declaration;
                    $this->constants[$declaration['fqcn']] = [];
                    foreach ($declaration['constants'] as $constant) {
                        $this->constants[$declaration['fqcn']][$constant['name']] = null;
                    }
                }
                continue;
            }
            if ($node instanceof Node\Stmt\GroupUse || $node instanceof Node\Stmt\Use_) {
                continue;
            }
        }
    }

    private function declaration(Node $node, string $sourceRoot, string $file, string $namespace): ?array
    {
        $name = $node->name?->toString();
        if ($name === null || $name === '') {
            return null;
        }
        $fqcn = $node->getAttribute('namespacedName')?->toString() ?? trim($namespace . '\\' . $name, '\\');
        $relative = ltrim(str_replace($sourceRoot, '', $file), DIRECTORY_SEPARATOR);
        $relative = str_replace('\\', '/', $relative);
        $kind = $node instanceof Node\Stmt\Enum_ ? 'enum' : ($node instanceof Node\Stmt\Interface_ ? 'interface' : ($node instanceof Node\Stmt\Trait_ ? 'trait' : 'class'));
        $constants = [];
        $properties = [];
        $methods = [];
        $cases = [];
        $statements = $node instanceof Node\Stmt\Class_ || $node instanceof Node\Stmt\Interface_ || $node instanceof Node\Stmt\Enum_ || $node instanceof Node\Stmt\Trait_ ? $node->stmts : [];
        $traits = [];
        foreach ($statements as $statement) {
            if ($statement instanceof Node\Stmt\TraitUse) {
                foreach ($statement->traits as $trait) {
                    $traits[] = $trait->toString();
                }
            } elseif ($statement instanceof Node\Stmt\ClassConst) {
                foreach ($statement->consts as $constant) {
                    $constants[] = [
                        'name' => $constant->name->toString(),
                        'valueNode' => $constant->value,
                        'valueText' => $this->printer->prettyPrintExpr($constant->value),
                        'value' => null,
                        'line' => $constant->getStartLine(),
                    ];
                }
            } elseif ($statement instanceof Node\Stmt\Property) {
                foreach ($statement->props as $property) {
                    $properties[] = $this->property($statement, $property, $this->docText($statement));
                }
            } elseif ($statement instanceof Node\Stmt\ClassMethod) {
                $method = $this->method($statement);
                $methods[] = $method;
                $parameterDocs = $this->parameterDocTypes($method['doc'] ?? null);
                foreach ($statement->params as $parameter) {
                    if (($statement->name->toString() === '__construct') && ($parameter->flags & Node\Stmt\Class_::MODIFIER_PRIVATE || $parameter->flags & Node\Stmt\Class_::MODIFIER_PROTECTED || $parameter->flags & Node\Stmt\Class_::MODIFIER_PUBLIC)) {
                        if (!in_array($parameter->var->name, array_column($properties, 'name'), true)) {
                            $properties[] = $this->promotedProperty($parameter, $parameterDocs[$parameter->var->name] ?? null);
                        }
                    }
                }
            } elseif ($statement instanceof Node\Stmt\EnumCase) {
                $cases[] = [
                    'name' => $statement->name->toString(),
                    'valueNode' => $statement->expr,
                    'valueText' => $statement->expr === null ? null : $this->printer->prettyPrintExpr($statement->expr),
                    'value' => null,
                    'line' => $statement->getStartLine(),
                ];
            }
        }

        $parent = null;
        $implements = [];
        if ($node instanceof Node\Stmt\Class_) {
            $parent = $node->extends?->toString();
            $implements = array_map(static fn (Node\Name $name): string => $name->toString(), $node->implements);
        } elseif ($node instanceof Node\Stmt\Interface_) {
            $implements = array_map(static fn (Node\Name $item): string => $item->toString(), $node->extends);
        }
        if ($kind === 'interface') {
            foreach ($methods as &$method) {
                $method['abstract'] = true;
            }
            unset($method);
        }

        $declaration = [
            'name' => $name,
            'fqcn' => $fqcn,
            'kind' => $kind,
            'backingType' => $node instanceof Node\Stmt\Enum_ ? $this->typeName($node->scalarType) : null,
            'namespace' => $namespace,
            'file' => $relative,
            'line' => $node->getStartLine(),
            'parent' => $parent,
            'implements' => $implements,
            'traits' => $traits,
            'abstract' => $node instanceof Node\Stmt\Class_ && $node->isAbstract(),
            'final' => $node instanceof Node\Stmt\Class_ && $node->isFinal(),
            'constants' => $constants,
            'cases' => $cases,
            'properties' => $properties,
            'methods' => $methods,
            'networkId' => null,
        ];
        foreach ($constants as $constant) {
            if ($constant['name'] === 'NETWORK_ID') {
                $declaration['networkIdNode'] = $constant['valueNode'];
                $declaration['networkIdText'] = $constant['valueText'];
            }
        }
        return $declaration;
    }

    private function property(Node\Stmt\Property $statement, Node\Stmt\PropertyProperty $property, ?string $doc): array
    {
        return [
            'name' => $property->name->toString(),
            'type' => $this->typeName($statement->type),
            'defaultNode' => $property->default,
            'defaultText' => $property->default === null ? null : $this->printer->prettyPrintExpr($property->default),
            'doc' => $doc,
            'static' => ($statement->flags & Node\Stmt\Class_::MODIFIER_STATIC) !== 0,
            'line' => $property->getStartLine(),
        ];
    }

    private function promotedProperty(Node\Param $parameter, ?string $docType = null): array
    {
        return [
            'name' => $parameter->var->name,
            'type' => $this->typeName($parameter->type),
            'defaultNode' => $parameter->default,
            'defaultText' => $parameter->default === null ? null : $this->printer->prettyPrintExpr($parameter->default),
            'doc' => $docType === null ? null : '/** @var ' . $docType . ' */',
            'static' => false,
            'line' => $parameter->getStartLine(),
        ];
    }

    private function parameterDocTypes(?string $doc): array
    {
        if ($doc === null || $doc === '') {
            return [];
        }

        $result = [];
        preg_match_all('/@(?:phpstan-)?param\s+(.+?)\s+\$([A-Za-z_][A-Za-z0-9_]*)/', $doc, $matches, PREG_SET_ORDER);
        foreach ($matches as $match) {
            $type = trim((string) $match[1]);
            $type = preg_replace('/\s+\*\/?\s*$/', '', $type) ?? $type;
            $result[(string) $match[2]] = trim($type);
        }
        return $result;
    }

    private function method(Node\Stmt\ClassMethod $method): array
    {
        $parameters = [];
        foreach ($method->params as $parameter) {
            $parameters[] = [
                'name' => $parameter->var->name,
                'type' => $this->typeName($parameter->type),
                'defaultNode' => $parameter->default,
                'defaultText' => $parameter->default === null ? null : $this->printer->prettyPrintExpr($parameter->default),
                'variadic' => $parameter->variadic,
                'nullable' => $parameter->type instanceof Node\NullableType,
            ];
        }
        return [
            'name' => $method->name->toString(),
            'visibility' => ($method->flags & Node\Stmt\Class_::MODIFIER_PRIVATE) !== 0 ? 'private' : (($method->flags & Node\Stmt\Class_::MODIFIER_PROTECTED) !== 0 ? 'protected' : 'public'),
            'static' => ($method->flags & Node\Stmt\Class_::MODIFIER_STATIC) !== 0,
            'abstract' => ($method->flags & Node\Stmt\Class_::MODIFIER_ABSTRACT) !== 0,
            'returnType' => $this->typeName($method->returnType),
            'parameters' => $parameters,
            'body' => $method->stmts,
            'line' => $method->getStartLine(),
            'doc' => $this->docText($method),
        ];
    }

    private function docText(Node $node): ?string
    {
        $comment = $node->getDocComment();
        return $comment === null ? null : $comment->getText();
    }

    private function typeName(?Node $type): ?string
    {
        if ($type === null) {
            return null;
        }
        if ($type instanceof Node\NullableType) {
            return '?' . ($this->typeName($type->type) ?? 'mixed');
        }
        if ($type instanceof Node\UnionType) {
            return implode('|', array_map(fn (Node $item): string => $this->typeName($item) ?? 'mixed', $type->types));
        }
        if ($type instanceof Node\IntersectionType) {
            return implode('&', array_map(fn (Node $item): string => $this->typeName($item) ?? 'mixed', $type->types));
        }
        return $type instanceof Node\Identifier || $type instanceof Node\Name ? $type->toString() : null;
    }
}
