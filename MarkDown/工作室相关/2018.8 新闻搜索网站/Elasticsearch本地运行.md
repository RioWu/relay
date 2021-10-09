# Elasticsearch本地运行

- 安装时遇到如下报错

  ```
  此时不应有 \Java\jdk1.8.0_181\bin\java.exe" -cp "!ES_CLASSPATH!" "org.elasti……
  ```

  - 解决方法：将java和Elasticsearch都放在一个没有空格、括号等特殊字符的目录下

- mongodb以副本集运行

  - `mongod -config D:\CODE\wamp64\www\riochen\southeast_news\mongo-es\mongod.conf`

- 运行Elasticsearch
  - 本地测试的版本是6.4.2，是目前Elasticsearch官方发布的最新版本
  - 需要注意的是，最新版本可能存在一些问题
    - 出现问题时解决方案偏少
    - 6.x版本的Elasticsearch不再支持一个index下可以有多个type的操作
  - 最终可能采用的版本是5.6，还没有确定

- 运行mongo-es 同步数据

  - `NODE_ENV=dev mongo-es D:/CODE/wamp64/www/riochen/southeast_news/mongo-es/config.json`

  - 错误总结

    - 运行时经常出现数据导入到一半Elasticsearch突然崩溃然后关闭，查看日志发现是因为内存分配过少，可以在 `config/jvm.option`里修改为jvm分配的内存。

    - 一开始想实现将Mongodb中的 id 映射到Elasticsearch中的 _id ，后来发现会报错

       ` Field [_id] is a metadata field and cannot be added inside a document. Use the index API request parameters.` 遂放弃

- 本地数据

  - liputan6 :印度尼西亚
  - metrotv:印度尼西亚
  - sindonews:印度尼西亚
  - republika:印度尼西亚

- 参考

  -  Elasticsearch各种分页案例
  -  [Elasticsearch search after分页查询案例分享](https://my.oschina.net/bboss/blog/1796241)
  -  [Elasticsearch Scroll和Slice Scroll查询API使用案例](https://my.oschina.net/bboss/blog/1942562)
  -  [Elasticsearch Sliced Scroll分页检索案例分享](https://my.oschina.net/bboss/blog/1788729)
  -  [Elasticsearch Scroll分页检索案例分享](https://my.oschina.net/bboss/blog/1786493)
  -  [Elasticsearch From-Size分页案例](https://my.oschina.net/bboss/blog/1829552)
  -  [一组获取Elasticsearch 索引表所有文档API使用案例](https://my.oschina.net/bboss/blog/2245872)
  -  [Elasticsearch SQL案例介绍](https://my.oschina.net/bboss/blog/2247795 )

- 搜索时用到的query语句

```json
{
    "from": 1,
    "size": 10,
    "query": {
        "bool": {
            "must": [
                {
                    "multi_match": {
                        "query": "test",
                        "fields": [
                            "author",
                            "abstract",
                            "news_content",
                            "news_title"
                        ]
                    }
                }
            ],
            "filter": [
                {
                    "range": {
                        "public_date": {
                            "gte": "2018-10-1",
                            "lt": "2018-10-10"
                        }
                    }
                }
            ]
        }
    },
    "highlight": {
        "fields": {
            "author": {},
            "abstract": {},
            "news_content": {},
            "news_title": {}
        }
    },
    "sort": [
        {
            "public_date": {
                "order": "desc"
            }
        }
    ]
}
```

# php接口设计

- 使用composer安装elasticsearch-php

- 具体实现

  - **在 elasticsearch-php 中，几乎一切操作都是用关联数组来配置。**

  - [如何处理json数组或对象（尤其是空对象和对象数组的问题）](https://www.elastic.co/guide/cn/elasticsearch/php/current/php_json_objects.html)	

  - 空对象：如果直接用

    ```json
    "content" ：{}
    ```

    这种形式，php会自动把 `"content":{}`转换成 `"content":[]`,这在Elasticsearch DSL中是非法的，定义空对象需要用

    ```php
    'content' => new stdClass;
    ```

  - 对象数组：可以用嵌套数组的形式实现

  - PHP5.4 及以上版本，可以使用 `[]` 构建数组。这会让多维数组看起来易读些：

    ```php
    $params['body'] = [
        'query' => [
            'match' => [
                'content' => 'quick brown fox'
            ]
        ],
        'sort' => [
            ['time' => ['order' => 'desc']],
            ['popularity' => ['order' => 'desc']]
        ]
    ];
    $results = $client->search($params);
    ```

    # Elasticsearhc 常用查询

    ```json
    //数据聚合 根据索引
    GET /Indonesia_liputan6,Indonesia_republika,vietnam_test/_search
    {
      "size": 0,
      "query": {
        "bool": {
          "must": [
            {
              "multi_match": {
                "query": "ketika dikonfirmasi",
                "fields": [
                  "author",
                  "abstract",
                  "news_content",
                  "news_title"
                ]
              }
            }
          ]
        }
      },
      "aggs": {
        "test": {
          "terms": {
            "field": "_index",
            "size": 10
          }
        }
      }
    }
    //数据聚合 根据时间
    GET /Indonesia_liputan6,Indonesia_republika/_search
    {
      "size": 0, 
          "query": {
            "bool": {
                "must": [
                    {
                        "multi_match": {
                            "query": "ketika dikonfirmasi",
                            "fields": [
                                "author",
                                "abstract",
                                "news_content",
                                "news_title"
                            ]
                        }
                    }
                ]
            }
        },
      "aggs": {
        "test": {
          "date_histogram": {
            "field": "public_date",
            "interval": "year", 
            "format": "yyyy-MM-dd"
          }
        }
      }
    }
    //数据搜素
    GET /indonesia_liputan6,indonesia_republika/_search
    {
        "from": 1,
        "size": 10,
        "query": {
            "bool": {
                "must": [
                    {
                        "multi_match": {
                            "query": "ketika dikonfirmasi",
                            "fields": [
                                "author",
                                "abstract",
                                "news_content",
                                "news_title"
                            ]
                        }
                    }
                ],
                "filter": [
                    {
                        "range": {
                            "public_date": {
                                "gte": "2000-10-01",
                                "lt": "2018-10-10"
                            }
                        }
                    }
                ]
            }
        },
        "highlight": {
            "fields": {
                "author": {},
                "abstract": {},
                "news_content": {},
                "news_title": {}
            }
        },
        "sort": [
            {
                "public_date": {
                    "order": "desc"
                }
            }
        ]
    }
    //全部数据
    GET /_search 
    {
      "query": {
        "match_all": {}
      }
    }
    //分片查询
    GET /_cat/shards
    GET /_cat/indices?pretty
    //单索引搜索
    GET /indonesia_liputan6/_search
    GET /indonesia_republika/_search
    GET /vietnam_test/_search
    //索引迁移
    POST _reindex
    {
      "source": {
        "index": "southeast_news"
      },
      "dest": {
        "index": "indonesia_liputan6"
      }
    }
    //索引结构查询
    GET /indonesia_liputan6/
    GET /indonesia_republika/
    
    ```

