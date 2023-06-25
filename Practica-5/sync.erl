-module(sync).

-export([createLock/0, lock/1, unlock/1, destroyLock/1]).
-export([createSem/1, semP/1, semV/1, destroySem/1]).
-export([testLock/0, testSem/0]).

lockHandler () ->
  receive
    {From, lock} ->
      From ! {self(), ok},
      receive
        {From, unlock} -> 
          lockHandler()
      end;
    dest -> ok 
  end.

createLock () -> 
  spawn (fun() -> lockHandler() end).

lock (L) -> 
  L ! {self(), lock},
  receive
    {L, ok} -> ok
  end.

unlock (L) -> 
  L ! {self(), unlock}.

destroyLock (L) -> 
  L ! dest.


semHandler(0) ->
  receive
    {From, semV} ->
      receive
        {_, semP} ->
          From ! {self(), ok},
          semHandler(0)
      end;
    {_, semP} ->
      semHandler(1);
    dest -> 
      ok
  end;

semHandler(N) ->
  receive
    {From, semV} -> 
      From ! {self(), ok},
      semHandler(N - 1);
    {_, semP} -> 
      semHandler(N + 1);
    dest -> 
      ok
  end.

createSem (N) -> 
  spawn (fun() -> semHandler(N) end).

destroySem (S) ->
  S ! dest.

semP (S) -> 
  S ! {self(), semP}.

semV (S) -> 
  S ! {self(), semV},
  receive
    {S, ok} -> 
      ok 
  end.

f (L, W) ->
  lock(L),
  % regioncritica(),
  io:format("uno ~p~n", [self()]),
  io:format("dos ~p~n", [self()]),
  io:format("tre ~p~n", [self()]),
  io:format("cua ~p~n", [self()]),
  unlock(L),
  W ! finished.

waiter (L, 0) -> 
  destroyLock(L);

waiter (L, N) -> 
  receive 
    finished -> 
      waiter(L, N-1) 
  end.

waiter_sem (S, 0) -> 
  destroySem(S);

waiter_sem (S, N) -> 
  receive 
    finished -> 
      waiter_sem(S, N-1) 
  end.

testLock () ->
  L = createLock(),
  W = spawn(fun () -> waiter(L, 3) end),
  spawn (fun () -> f(L, W) end),
  spawn (fun () -> f(L, W) end),
  spawn (fun () -> f(L, W) end),
  ok.

sem (S, W) ->
  semV(S),
  %regioncritica(), bueno, casi....
  io:format("uno ~p~n", [self()]),
  io:format("dos ~p~n", [self()]),
  io:format("tre ~p~n", [self()]),
  io:format("cua ~p~n", [self()]),
  io:format("cin ~p~n", [self()]),
  io:format("sei ~p~n", [self()]),
  semP(S),
  W ! finished.

testSem () ->
  S = createSem(2), % a lo sumo dos usando io al mismo tiempo
  W = spawn (fun () -> waiter_sem (S, 5) end),
  spawn (fun () -> sem (S, W) end),
  spawn (fun () -> sem (S, W) end),
  spawn (fun () -> sem (S, W) end),
  spawn (fun () -> sem (S, W) end),
  spawn (fun () -> sem (S, W) end),
  ok.
