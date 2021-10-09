<?php
require "vendor/autoload.php";
use Elasticsearch\ClientBuilder;
use Detection\MobileDetect;
$detect = new MobileDetect();
/**
 * @author RioChen<283489710@qq.com>
 * 用于获取当前时间
 */
function get_time()
{
    list($msec, $sec) = explode(" ", microtime());
    return (float)$msec + (float)$sec;
}
/**
 * @author RioChen<283489710@qq.com>
 * @param $client是传入的一个elasticsearch client，$json_array为搜索提供具体信息
 * @param $json_array['index_string'] 表示要进行搜索的索引列表,是通过传入的language和index两个属性处理得到的
 * @param $json_array['date_range'] 表示搜索的时间范围
 * @param $json_array['searchMode'] 表示搜素模式
 * @param $json_array['query'] 表示要搜索的内容
 * 用于进行各个网站的结果数量统计
 */
function count_aggs($client, $json_array)
{
    $count_params = [
        'index' => $json_array['index_string'],
        'type' => 'news_data',
        'body' => [
            'size' => 0,
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
                                    'lt' => end($json_array['dateRange'])
                                ]
                            ]
                        ]
                    ]
                ]
            ],
            'aggs' => [
                'aggs_by_index' => [
                    'terms' => [
                        'field' => '_index',
                        'size' => 10
                    ]
                ]
            ]
        ]
    ];
    if ($json_array["searchMode"] == "and") {
        $count_params["body"]["query"]["bool"]["must"][0]["multi_match"]["type"] == "phrase";
    }
    $count_result = $client->search($count_params);
    $count_result = $count_result['aggregations']['aggs_by_index']['buckets'];
    foreach ($count_result as &$value) {
        $value['item'] = $value['key'];
        $value['count'] = $value['doc_count'];
        unset($value['key']);
        unset($value['doc_count']);
    }
    unset($value);

    return $count_result;
}


/**
 * @author RioChen<283489710@qq.com>
 * @param $client是传入的一个elasticsearch client，$json_array为搜索提供具体信息
 * @param $index 表示要进行查询的具体网站，因为这个函数是返回单个网站的各个时间段的信息，所以每次只会传入一个索引
 * @param $json_array['date_range'] 表示搜索的时间范围
 * @param $json_array['searchMode'] 表示搜素模式
 * @param $json_array['query'] 表示要搜索的内容
 * @param $json_array['timeDivide'] 表示要分割的份数
 * 用于进行单个网站的各个时间段的数量统计
 */
function time_aggs($client, $json_array, $index, $detect)
{
    //如果用户没有选择时间，会使用默认时间1000-01-01和3000-01-01，这个时候不能简单的对date_range的信息进行处理了，要通过请求得到对应的有数据的最大时间和最小时间
    if ($json_array["dateRange"][0] == "1000-01-01") {
        $time_params = [
            'index' => $json_array['index_string'],
            'type' => 'news_data',
            'body' => [
                'size' => 0,               //每页返回10个搜索结果
                'query' => [
                    'bool' => [
                        'must' => [
                            [
                                'multi_match' => [
                                    'query' => $json_array['query'],
                                    'fields' => ['author', 'abstract', 'news_content', 'news_title']
                                ]
                            ],
                        ]
                    ]
                ],
                "aggs" => [
                    "min" => [
                        "min" => [
                            "field" => "public_date"
                        ]
                    ],
                    "max" => [
                        "max" => [
                            "field" => "public_date"
                        ]
                    ]
                ]
            ]
        ];
        $time_result = $client->search($time_params);
        $json_array["dateRange"][0] = substr($time_result["aggregations"]["min"]["value_as_string"], 0, 10);
        $json_array["dateRange"][1] = substr($time_result["aggregations"]["max"]["value_as_string"], 0, 10);
    }
    $time_params = [
        'index' => $index,
        'type' => 'news_data',
        'body' => [
            //'from' => $json_array['from'],
            'size' => 0,               //每页返回10个搜索结果
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
                                    'gte' => '',
                                    'lt' => ''
                                ]
                            ]
                        ]
                    ]
                ]
            ]
        ]
    ];


    if ($json_array["searchMode"] == "and") {
        $time_params["body"]["query"]["bool"]["must"][0]["multi_match"]["type"] == "phrase";
    }

    /*$endDate = strtotime($json_array['dateRange'][1]);
    $beginDate = strtotime($json_array['dateRange'][0]);
    $timeDivide = ceil(($endDate - $beginDate) / ($json_array['timeDivide'] + 1));    //向上取整，计算时间间隔
    $divideResult = [];
    $count = 0;
    //根据要分成的份数具体统计，分成的份数等于$timeDivide+1
    for ($i = 0; $i < $json_array['timeDivide'] + 1; $i++) {
        $beginTime = date("Y-m-d", $beginDate + $i * $timeDivide);
        $endTime = date("Y-m-d", $beginDate + ($i + 1) * $timeDivide);
        $time_params["body"]["query"]["bool"]["filter"][0]["range"]["public_date"]["gte"] = $beginTime;
        $time_params["body"]["query"]["bool"]["filter"][0]["range"]["public_date"]["lt"] = $endTime;
        $time_result = $client->search($time_params);
        $divideResult[$i]['item'] = $beginTime . "~" . $endTime;
        $divideResult[$i]['count'] = $time_result["hits"]["total"];
        $count += $divideResult[$i]['count'];
    }*/
    //这里存的是之后修改需求后对应的代码，即根据用户指定的时间分段进行时间统计，期望前端传回来的格式，'dateRange'是一个数组，包括用户选定的几个时间,包括最开始的时间和最后的时间
    $count = 0;
    for ($i = 0; $i < count($json_array['dateRange']) - 1; $i++) {
        $beginTime = $json_array['dateRange'][$i];
        $endTime = $json_array['dateRange'][$i + 1];
        $time_params["body"]["query"]["bool"]["filter"][0]["range"]["public_date"]["gte"] = $beginTime;
        $time_params["body"]["query"]["bool"]["filter"][0]["range"]["public_date"]["lt"] = $endTime;
        $time_result = $client->search($time_params);
        if ($detect->isMobile()) {
            $divideResult[$i]['item'] = $beginTime ;
        } else {
            $divideResult[$i]['item'] = $beginTime. "~" . $endTime;
        }
        $divideResult[$i]['count'] = $time_result["hits"]["total"];
        $count += $divideResult[$i]['count'];
    }
    if ($count == 0) {
        return false;
    }
    return $divideResult;
}


$json_array = json_decode(file_get_contents("php://input"), true);      //获取前端传输的数据
$time_start = get_time();
global $index_string, $time_result, $i;
$client = ClientBuilder::create()->build();
$json_array['language'] = lcfirst($json_array['language']);     //语言的首字母大写,以匹配搜索引擎中的索引名

//构建用于搜索的index_string
foreach ($json_array['index'] as &$value) {
    $value = $json_array['language'] . '_' . $value;
    $time_result[$i] = $value;
    $index_string .= ',' . $value;      //构造表示搜索的索引的字符串
    $i++;
}
unset($value);
$index_string = ltrim($index_string, ',');         //删除开头的逗号
$json_array['index_string'] = $index_string;


$result = [];
if ($json_array['type'] == "time") {
//通过前端传递的language和index两个数据来构建用于搜索的索引字符串
    $i = 0;
    $time_result = $json_array['index'];
    foreach ($time_result as $key => $value) {
        $time_return = time_aggs($client, $json_array, $value,$detect);
        if ($time_return != false) {                    //此时$time_result以索引数组的形式存放查找的索引名
            $time_result[$value] = $time_return;        //相当于把$time_result中的value当作键名再创建几个键值对，然后删掉之前无用的key
        }
        unset($time_result[$key]);
    }
    $result['time_result'] = $time_result;
}
$count_result = count_aggs($client, $json_array);
$result['count_result'] = $count_result;
$time_end = get_time();
$time = $time_end - $time_start;
$result['time'] = $time;
echo json_encode($result, JSON_UNESCAPED_UNICODE);
?>
