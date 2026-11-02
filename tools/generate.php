<?php

declare(strict_types=1);

$root = dirname(__DIR__);
$autoload = $root . DIRECTORY_SEPARATOR . 'vendor' . DIRECTORY_SEPARATOR . 'autoload.php';
if (!is_file($autoload)) {
    fwrite(STDERR, "Composer dependencies are missing. Run composer install first.\n");
    exit(1);
}

require $autoload;

use kodiak\bedrockprotocol\generator\application\GeneratorApplication;
use kodiak\bedrockprotocol\generator\parser\ProtocolSourceParser;
use kodiak\bedrockprotocol\generator\source\GitSourceRepository;
use kodiak\bedrockprotocol\generator\transform\DeclarationTransformer;
use kodiak\bedrockprotocol\generator\transform\ModelTransformationPipeline;
use kodiak\bedrockprotocol\generator\transform\PacketTransformer;

$application = new GeneratorApplication(
    new GitSourceRepository(),
    new ProtocolSourceParser(),
    new ModelTransformationPipeline([
        new DeclarationTransformer(),
        new PacketTransformer(),
    ]),
);

exit($application->run($root, array_slice($argv, 1)));
