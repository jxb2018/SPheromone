SPheromone
---

Simple Pheromone

## Install dependencies

```shell
sudo apt-get install libyaml-cpp-dev
sudo apt-get install libprotobuf-dev protobuf-compiler
sudo apt-get install -y libzstd-dev libssl-dev libz-dev

# mongo-c-driver
sudo apt-get install -y libmongoc-dev libbson-dev libbson-1.0-0

# boost
sudo apt install libboost-all-dev
```

## Build

set `Build Directory` ad `/tmp/SPheromone129`, and then build

## Run

ref `benchmarks/01`

## Benchmarks

<pre><code>
benchmarks
    |---00 # application: long function chain
    |---01 # application: test transfer data
    |---02 # real serverless application: media service
    |---03 # real serverless application: ml pipeline
    |---04 # real serverless application: finra
|---Lumine
</code></pre>
