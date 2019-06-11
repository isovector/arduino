{-# LANGUAGE FlexibleInstances    #-}
{-# LANGUAGE TypeSynonymInstances #-}

module Data.C.Types where

import Data.Proxy
import Data.C.Expr

------------------------------------------------------------------------------


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
  fromInteger x = uniform . show $ (fromInteger x :: Int)


------------------------------------------------------------------------------


type CFloat = Expr 'CTFloat

instance ToCType 'CTFloat where
  tag = Proxy
  toCType _ = CTFloat

instance Num CFloat where
  (+) = op2' "+"
  (-) = op2' "-"
  (*) = op2' "*"
  abs = op1pre "abs"
  signum = op1pre "sign"
  negate = op1 "-"
  fromInteger x = uniform . show $ (fromInteger x :: Float)


instance Fractional CFloat where
  (/) = op2' "/"
  fromRational x = uniform . show $ (fromRational x :: Float)

instance Floating CFloat where
  -- pi = copy $ uniform "pi"
  pi = uniform "3.141592653589793238462643383"
  exp = op1pre "exp"
  log = op1pre "log"
  sqrt = op1pre "sqrt"
  (**) = op2pre' "pow"
  sin = op1pre "sin"
  cos = op1pre "cos"
  tan = op1pre "tan"
  asin = op1pre "asin"
  acos = op1pre "acos"
  atan = op1pre "atan"
  sinh x = (exp x - exp (negate x)) / 2
  cosh x = (exp x + exp (negate x))/2
  tanh x = sinh x / cosh x
  asinh x = log $ x + sqrt(x**2 + 1)
  acosh x = log $ x + sqrt(x**2 - 1)
  atanh x = 0.5 * log ((1 + x)/(1 - x))


--------------------------------------------------------------------------------

type CRGB = Expr 'CTRGB

instance ToCType 'CTRGB where
  tag = Proxy
  toCType _ = CTRGB

rgb :: CInt -> CInt -> CInt -> CRGB
rgb = op3pre "CRGB"


--------------------------------------------------------------------------------

type CVoid = Expr 'CTVoid

instance ToCType 'CTVoid where
  tag = Proxy
  toCType _ = CTVoid


showNumber :: CInt -> CVoid
showNumber = op1pre "showNumber"

