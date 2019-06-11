{-# LANGUAGE BlockArguments  #-}
{-# LANGUAGE RecursiveDo     #-}
{-# LANGUAGE TemplateHaskell #-}

module Polysemy.State.Final where

import Polysemy
import Polysemy.Fixpoint
import Polysemy.State

data FinalState s m a where
  GetFinal :: FinalState s m s

makeSem ''FinalState


runFinalState
    :: Member Fixpoint r
    => s
    -> Sem (State s ': FinalState s ': r) a
    -> Sem r a
runFinalState s m = mdo
  ~(s', a) <-
    interpret
      \case
        GetFinal -> pure s'
      (runLazyState s m)
  pure a

