-module(turnos_pp).
-export([start/0, close/0]).

% puts(String) ->
%   io:fwrite(String).

start() ->
  {ok, ListenSocket} = gen_tcp:listen(8000, [{reuseaddr, true}]),
  register(cnt, spawn (fun () -> connection(ListenSocket) end)),
  Counter = counters:new(1, []), %Inicia un contador (Array de un solo elemento de un contador).
  register(srv, spawn (fun () -> server(#{}) end)),
  Pid = spawn (fun() -> wait_connect(ListenSocket, 0, Counter, srv) end),
  srv ! {Pid, {add, 0}},
  ok.

close() ->
  srv ! {close, listen},
  ok.

close_listen() ->
  cnt ! fin,
  unregister(cnt).

connection(Socket) ->
  receive
    fin -> gen_tcp:close(Socket);
    _ -> connection(Socket)
  end.

server(PidMap) ->
  receive
    {From, {add, N}} ->
      server(maps:put(N, From, PidMap));
    {_, {del, N}} ->
      server(maps:remove(N, PidMap));
    {close, listen} ->
      close_listen(),
      server(PidMap);
    {close, map} ->
      Pids = maps:values(PidMap),
      lists:map(fun close_client/1, Pids),
      unregister(srv)
  end.

close_client(Pid) ->
  Pid ! close,
  ok.

wait_connect(ListenSocket, N, Counter, Server) ->
  {Status, Msg} = gen_tcp:accept(ListenSocket),
  if
    Status == ok ->
      Server ! {spawn (fun () -> wait_connect(ListenSocket, N+1, Counter, Server) end), {add, N+1}},
      get_request(Msg, N, Counter, ListenSocket);
    Status == error ->
      if
        Msg == closed -> 
          io:fwrite("Socket de escucha cerrado~n"),
          Server ! {self(), {del, N}},
          Server ! {close, map};
        Msg == system_limit -> 
          io:fwrite("Limite de sockets exedidos.~n"),
          Server ! {self(), {del, N}},
          Server ! {close, map}
      end
  end.

get_request(Socket, N, Counter, Server) ->
  io:fwrite("Esperando mensajes de ~p~n", [N]),
  receive
    {tcp, Socket, "NUEVO\n"} -> 
      handler(Socket, Counter),
      get_request(Socket, N, Counter, Server);
    {tcp, Socket, "CHAU\n"} -> 
      io:fwrite("Adios !!!~n"),
      gen_tcp:close(Socket),
      Server ! {self(), {del, N}};
    {tcp_closed, Socket} -> 
      io:fwrite("Conexion con cliente: ~p terminada~n", [Socket]),
      gen_tcp:close(Socket);
    {tcp, Socket, _} -> 
      gen_tcp:send(Socket, "Comando incorrecto\n"),
      get_request(Socket, N, Counter, Server);
    close -> 
      io:fwrite("Cierre del servidor~n"), 
      gen_tcp:close(Socket)

  end. 

handler(Socket, Counter) ->
  counters:add(Counter, 1, 1),
  gen_tcp:send(Socket, erlang:integer_to_list(counters:get(Counter, 1))),
  gen_tcp:send(Socket, "\n").
