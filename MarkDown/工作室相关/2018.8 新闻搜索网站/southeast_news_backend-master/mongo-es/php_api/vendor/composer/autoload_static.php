<?php

// autoload_static.php @generated by Composer

namespace Composer\Autoload;

class ComposerStaticInit69e7020cf6c95c5361dfb05ab14207b6
{
    public static $files = array (
        'ad155f8f1cf0d418fe49e248db8c661b' => __DIR__ . '/..' . '/react/promise/src/functions_include.php',
    );

    public static $prefixLengthsPsr4 = array (
        'R' => 
        array (
            'React\\Promise\\' => 14,
        ),
        'P' => 
        array (
            'Psr\\Log\\' => 8,
        ),
        'G' => 
        array (
            'GuzzleHttp\\Stream\\' => 18,
            'GuzzleHttp\\Ring\\' => 16,
        ),
        'E' => 
        array (
            'Elasticsearch\\' => 14,
        ),
    );

    public static $prefixDirsPsr4 = array (
        'React\\Promise\\' => 
        array (
            0 => __DIR__ . '/..' . '/react/promise/src',
        ),
        'Psr\\Log\\' => 
        array (
            0 => __DIR__ . '/..' . '/psr/log/Psr/Log',
        ),
        'GuzzleHttp\\Stream\\' => 
        array (
            0 => __DIR__ . '/..' . '/guzzlehttp/streams/src',
        ),
        'GuzzleHttp\\Ring\\' => 
        array (
            0 => __DIR__ . '/..' . '/guzzlehttp/ringphp/src',
        ),
        'Elasticsearch\\' => 
        array (
            0 => __DIR__ . '/..' . '/elasticsearch/elasticsearch/src/Elasticsearch',
        ),
    );

    public static $prefixesPsr0 = array (
        'D' => 
        array (
            'Detection' => 
            array (
                0 => __DIR__ . '/..' . '/mobiledetect/mobiledetectlib/namespaced',
            ),
        ),
    );

    public static $classMap = array (
        'Mobile_Detect' => __DIR__ . '/..' . '/mobiledetect/mobiledetectlib/Mobile_Detect.php',
    );

    public static function getInitializer(ClassLoader $loader)
    {
        return \Closure::bind(function () use ($loader) {
            $loader->prefixLengthsPsr4 = ComposerStaticInit69e7020cf6c95c5361dfb05ab14207b6::$prefixLengthsPsr4;
            $loader->prefixDirsPsr4 = ComposerStaticInit69e7020cf6c95c5361dfb05ab14207b6::$prefixDirsPsr4;
            $loader->prefixesPsr0 = ComposerStaticInit69e7020cf6c95c5361dfb05ab14207b6::$prefixesPsr0;
            $loader->classMap = ComposerStaticInit69e7020cf6c95c5361dfb05ab14207b6::$classMap;

        }, null, ClassLoader::class);
    }
}
