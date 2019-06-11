{-# LANGUAGE BlockArguments     #-}
{-# LANGUAGE DerivingStrategies #-}
{-# LANGUAGE DerivingVia        #-}
{-# LANGUAGE RecursiveDo        #-}
{-# LANGUAGE TemplateHaskell    #-}

module Lib where

import Polysemy
import Polysemy.Fixpoint
import Polysemy.State
import Polysemy.State.Final

newtype NoQuotes = NoQuotes String

instance Show NoQuotes where
  show (NoQuotes s) = s

newtype Expr t = Expr { getExpr :: String }
  deriving (Eq, Ord)
  deriving Show via NoQuotes



frick
    :: ( Member (FinalState Int) r
       , Member (State Int) r
       )
    => Sem r Int
frick = do
  r <- getFinal
  modify (+1)
  modify (+1)
  modify (+1)
  pure r


someFunc :: IO ()
someFunc
  = print
  . run
  . runFixpoint run
  . runFinalState 0
  $ frick

