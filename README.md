# distributed_with_rpc
A project in Distributed Systems using RPC

## How to use
**Initial Setup**
- `make -f Makefile.server_funcs`
- `gcc -o client client.c`

**Run RPC Server**
- `./server_funcs_server`

**Run RPC Client (also the server in the server-client socket)**
- `./servers_funcs_client localhost 51717`

**Run Socket Client**
- `./client localhost 51717`
