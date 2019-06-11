{-# LANGUAGE FlexibleInstances    #-}
{-# LANGUAGE TypeSynonymInstances #-}

module Data.C.Types where

import Data.Proxy
import Data.C.Expr

type CInt = Expr 'CTInt

instance ToCType 'CTInt where
  tag = Proxy
  toCType _ = CTInt

instance Num CInt where
  (+) = op2' "+"
  (-) = op2' "-"
  (*) = op2' "*"
  abs = op1pre "abs"
  signum = op1pre "sign"
  negate = op1 "-"
  fromInteger x = uniform . show $ (fromInteger x :: Float)

