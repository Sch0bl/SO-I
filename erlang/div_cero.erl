-module(div_zero).
-export([atrapa/0]).


atrapa() ->
  case catch (4/0) of
  {'EXIT', {Reason, Stack}} -> io:fwrite("Division por cero ~n")
  end.
