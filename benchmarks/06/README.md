# EXP06: new version of FINRA

## Run

```shell
cd scripts
bash start_all.sh
```

## Invoke
```shell
bash invoke.sh
```

## change fanout num
在`scripts/start_all.sh`中，调用register时，第一个参数描述了注册的扇出数量
在‘scripts/invoke.sh’中，调用invoke时，第一个参数描述了调用的扇出数量
注意这两个参数最好保持一致
同时需要准备好足够的executor（修改start_all.sh和config/config.yml)