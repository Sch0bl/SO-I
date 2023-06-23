-module(anillos).
-export([start/1, close/1, get_sum/2]).

start(N) -> start_aux(N, N, maps:new()).

start_aux(0, _, Map) ->
  List = maps:values(Map),
  lists:map(fun(Pid) -> Pid ! {init, Map} end, List),
  Map;
start_aux(N, S, Map) ->
  Pid = spawn(fun() -> procs(N - 1, N - 1, S - 1, S, 0, Map) end),
  NewMap = maps:put(N - 1, Pid, Map),
  start_aux(N - 1, N, NewMap).

get_sum(Map, NroId) ->
  Pid = maps:get(NroId, Map),
  Pid ! {self(), get},
  receive
    {_, Sum} -> Sum
  end.

close(Map) ->
  List = maps:values(Map),
  lists:map(fun(Pid) -> Pid ! kill end, List),
  ok.

procs(NroId, Sum, Size, Counter, 0, _) ->
  receive
    {init, TheMap} -> procs(NroId, Sum, Size, Counter, 1, TheMap);
    kill -> ok 
  end;
procs(NroId, Sum, Size, Counter, 2, Map) ->
  receive
    {From, get} -> 
      From ! {self(), Sum},
      procs(NroId, Sum, Size, Counter, 2, Map) 
  end;
procs(0, Sum, Size, Counter, 1, Map) ->
  NextPid = maps:get(1, Map),
  PrevPid = maps:get(Size - 1, Map),
  NextPid ! {self(), Sum},
  receive
    {PrevPid, ToSum} -> procs(0, Sum + ToSum, Size, Counter - 1, 1, Map)
  end;
procs(Size, Sum, Size, Counter, 1, Map) ->
  NextPid = maps:get(0, Map),
  PrevPid = maps:get(Size - 2, Map),
  NextPid ! {self(), Sum},
  receive
    {PrevPid, ToSum} -> procs(Size, Sum + ToSum, Size, Counter - 1, i, Map)
  end;
procs(NroId, Sum, Size, Counter, 1, Map) ->
  NextPid = maps:get(NroId + 1, Map),
  PrevPid = maps:get(NroId - 1, Map),
  NextPid ! {self(), Sum},
  receive
    {PrevPid, ToSum} -> procs(NroId, Sum + ToSum, Size, Counter - 1, 1, Map)
  end;
procs(NroId, Sum, Size, 0, 1, Map) ->
  NextPid = maps:get(NroId + 1, Map),
  PrevPid = maps:get(NroId - 1, Map),
  NextPid ! {self(), Sum},
  receive
    {PrevPid, ToSum} -> procs(NroId, Sum + ToSum, Size, 0, 2, Map)
  end.
