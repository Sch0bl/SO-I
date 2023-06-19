-module(broadcast).

%% Libreía
% -funciones control
-export([start/0,stop/1]).
% Funciones intrección
-export([broadcast/2,registrar/1]).
-export([loopBroadcast/1]).
-export([test/0,client/1]).

start() -> 
  spawn(?MODULE, loopBroadcast, [[]]).

stop(Broadcaster) ->
  Broadcaster ! fin.

broadcast(Broadcaster, Msg) ->
  Broadcaster ! {env, Msg}.

registrar(Broadcaster) ->
  Broadcaster ! {reg, self()}.

loopBroadcast(Clientes) ->
  receive
    {fin} -> ok;
    {env, Msg} -> lists:foreach(fun(Client) -> Client ! Msg end, Clientes);
    {reg, Pid} -> 
      io:fwrite("client ~p registering...~n", [Pid]), 
      loopBroadcast([Pid | Clientes])
  end.

client(Broadcaster) ->
  registrar(Broadcaster),
  receive
    Msg -> io:fwrite("client ~p recibi: ~p ~n", [self(), Msg]) 
  end.
%% TODO -- entender el ejercicio

test() ->
  Broadcaster = broadcast:start(),
  spawn(?MODULE, client, [Broadcaster]),
  spawn(?MODULE, client, [Broadcaster]),
  broadcast(Broadcaster, hola).
