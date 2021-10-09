<?php
require "vendor/autoload.php";
use Elasticsearch\ClientBuilder;

$json_array = json_decode(file_get_contents("php://input"), true);
if (array_key_exists("searchMode", $json_array) == false) {
    $json_array["searchMode"] == "or";
}
$json_array['language'] = lcfirst($json_array['language']);


//通过前端传递的language和index两个数据来构建用于搜索的索引字符串
global $index_string;
foreach ($json_array['index'] as $value) {
    $value = $json_array['language'] . '_' . $value;
    $index_string .= ',' . $value;
}
$index_string = ltrim($index_string, ',');

$client = ClientBuilder::create()->build();                 //建立一个es客户端的实例
$params = [
    'index' => $index_string,
    'type' => 'news_data',
    'body' => [
        'size' => 1000,               //每页返回10个搜索结果
        'query' => [
            'bool' => [
                'must' => [
                    [
                        'multi_match' => [
                            'query' => $json_array['query'],
                            'fields' => ['author', 'abstract', 'news_content', 'news_title']
                        ]
                    ],
                ],
                'filter' => [
                    [
                        'range' => [
                            'public_date' => [
                                'gte' => $json_array['dateRange'][0],
                                'lt' => $json_array['dateRange'][1]
                            ]
                        ]
                    ]
                ]
            ]
        ],
        'highlight' => [
            'number_of_fragments' => '0',
            'fields' => [
                'author' => new stdClass,
                'abstract' => new stdClass,
                'news_content' => new stdClass,
                'news_title' => new stdClass
            ]
        ],
        'sort' => [
            [
                'public_date' => [
                    'order' => 'desc'
                ]
            ]
        ]
    ]
];
if ($json_array["searchMode"] == "and")
{
    $params["body"]["query"]["bool"]["must"][0]["multi_match"]["type"] == "phrase";
}
if ($json_array['sortMode'][0] == 'time') {
    $params['body']['sort'][0]['public_date']['order'] = $json_array['sortMode'][1];
} else if ($json_array['sortMode'][0] == 'score') {
    unset($params['body']['sort']);
}
$result = $client->search($params);
$result = $result['hits']['hits'];
global $count;
$count = 0;
foreach ($result as &$value) {
    $highlight = $value['highlight'];
    $value = $value['_source'];
    $value['highlight'] = $highlight;
    $value['newsid'] = $count;
    //unset($value['news_content']);
    $count++;
}
unset($value); //取消引用
$result_json = json_encode($result);
echo $result_json;
?>
