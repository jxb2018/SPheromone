ssh -p 22 lgw@192.168.1.129 "cmake --build /tmp/SPheromone129 \
--target manager coordinator scheduler executor \
exp06_invoker exp06_marginbalance exp06_marketdata exp06_register exp06_run_audit -- -j 16"