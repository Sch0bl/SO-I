-module(anillos).
-export([start/1, close/0, get_sum/1]).

%procs (NroId, Sum, Size, Counter, Map).
%NroId. Int.
%Sum. Int variable que guarda la suma.
%Size. Int Cantidad de procesos.
%Map. los procesos están guardados en un mapa.

%Caso Inicial, espera a que todos los procesos sean creados.
%Una vez recibida la señal init, con TheMap con todos los procesos,
%Manda al proceso siguiente su NroId para computar la 
%suma de todos.
procs(NroId, Sum, Size, Size, _) ->
  receive
    {init, TheMap} -> 
      NextPid = maps:get((NroId + 1) rem Size, TheMap),
      NextPid ! {self(), NroId},
      procs(NroId, Sum, Size, Size - 1, TheMap)
  end;

%Caso Base: La suma fue computada, espera la señal get para
%poder retornar el resultado.
%Con la señal kill se termina el proceso
procs(NroId, Sum, Size, 0, Map) ->
  receive
    {From, get} -> 
      From ! {self(), Sum},
      procs(NroId, Sum, Size, 0, Map);
    kill -> ok 
  end;

%Recursivo: Matching NroId == 0.
%Es el caso en el que NroId es 0, ya que NroId - 1 es negativo
procs(0, Sum, Size, Counter, Map) ->
  NextPid = maps:get(1, Map),
  PrevPid = maps:get(Size - 1, Map),
  receive
    {PrevPid, ToSum} -> 
      NextPid ! {self(), ToSum},
      procs(0, Sum + ToSum, Size, Counter - 1,Map)
  end;

%Recursivo: 
%Espera la señal del proceso anterior, cunado resive el sumando
%se la manda al proceso siguinte llama recursivamente la fución
%con la suma.
procs(NroId, Sum, Size, Counter, Map) ->
  NextPid = maps:get((NroId + 1) rem Size, Map),
  PrevPid = maps:get(NroId - 1, Map),
  receive
    {PrevPid, ToSum} -> 
      NextPid ! {self(), ToSum},
      procs(NroId, Sum + ToSum, Size, Counter - 1, Map)
  end.

%Caso base: Como los todos los procesos fueron creados los inicia
start_proc(0, _, Map) ->
  maps:map(fun(_,Pid) -> Pid ! {init, Map} end, Map),
  Map;

%Recursion: Genera un proceso, lo agrega al mapa
start_proc(N, S, Map) ->
  Pid = spawn(fun() -> procs(N - 1, N - 1, S, S, Map) end),
  NewMap = maps:put(N - 1, Pid, Map),
  start_proc(N - 1, S, NewMap).

%Servidor que mantiene activo el mapa de procesos
server(Map) ->
  receive
    %Obtención de sumas.
    {From, {get, NroId}} ->
      Pid = maps:get(NroId, Map),
      Pid ! {srv, get},
      receive
        {Pid, Sum} -> From ! {srv, Sum}
      end,
      server(Map);
    %Cierre del servidor.
    {From, close} ->
      maps:map(fun(_, Pid) -> Pid ! kill end, Map),
      From ! {srv, ok}
  end.

%Inicia el servidor que mantiene el mapa de pocesos.
start(N) -> 
  Map = start_proc(N, N, maps:new()),
  register(srv, spawn(fun() -> server(Map) end)),
  ok.

%start_proc(NroId, Size, Map)
%NroProc es el número de proceso a crear
%Size es la cantidad de procesos en total

%Obtiene la suma del proceso.
get_sum(NroId) ->
  srv ! {self(), {get, NroId}},
  receive
    {srv, Sum} -> Sum
  end.

%Cierra el servidor.
close() ->
  srv ! {self(), close},
  receive
    {srv, ok} -> ok 
  end.

