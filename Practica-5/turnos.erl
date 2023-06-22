-module(turnos).
-export([server/0]).

server() ->
  {ok, ListenSocket} = gen_tcp:listen(8000, [{reuseaddr, true}]),
  Counter = counters:new(1, []), %Inicia un contador (Array de un solo elemento de un contador).
  wait_connect(ListenSocket, 0, Counter).

wait_connect(ListenSocket, N, Counter) ->
  {ok, Socket} = gen_tcp:accept(ListenSocket),
  spawn (fun () -> wait_connect (ListenSocket, N+1, Counter) end),
  get_request(Socket, N, Counter, ListenSocket).

get_request(Socket, N, Counter, ListenSocket) ->
  io:fwrite("Esperando mensajes de ~p~n", [N]),
  receive
    {tcp, Socket, "NUEVO\n"} -> handler(Socket, Counter),
                                get_request(Socket, N, Counter, ListenSocket);
    {tcp_closed, Socket} -> io:fwrite("Conexion con cliente: ~p terminada~n", [Socket]);
                            % wait_connect(ListenSocket, N - 1, Counter);
    {tcp, Socket, _} -> gen_tcp:send(Socket, "Comando incorrecto\n"),
                        get_request(Socket, N, Counter, ListenSocket)

  end. 



handler(Socket, Counter) ->
  counters:add(Counter, 1, 1),
  gen_tcp:send(Socket, erlang:integer_to_list(counters:get(Counter, 1))),
  gen_tcp:send(Socket, "\n").
% hand_turnos(N) ->
%   reveive
%     fin -> ok;
%     inc -> turnos(N + 1);
%     {get, Pid} -> Pid ! {turno, N} 
%   end.


% manejador_mensaje(Data, Turnos) ->
%   if
%     Data == "NUEVO\n" -> Turnos ! {get, self()},
%     true -> 

