```
go run main.go
python3 ./bcc_example.py `pgrep -nx go`  > profile.out
./flamegraph.pl  profile.out > flamegraph.svg
```
