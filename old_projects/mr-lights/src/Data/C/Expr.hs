{-# LANGUAGE DeriveFunctor        #-}
{-# LANGUAGE FlexibleInstances    #-}
{-# LANGUAGE InstanceSigs         #-}
{-# LANGUAGE TypeSynonymInstances #-}

-- {-# OPTIONS_GHC -Wall #-}

module Data.C.Expr where

import Data.Functor.Compose
import Data.Reify
import Data.Proxy
import Data.Traversable
import Unsafe.Coerce



data SomeCType where
  SomeCType :: ToCType a => Expr a -> SomeCType



-- | Internal type tag
data CType
  = CTInt
  | CTRGB
  | CTFloat
  | CTBool
  | CTVoid
  deriving (Eq, Ord)

instance Show CType where
  show x = case x of
    CTFloat -> "float"
    CTBool  -> "bool"
    CTRGB   -> "CRGB"
    CTInt   -> "int"
    CTVoid  -> "void"

-- | Internal form tag
data ExprForm
  = Uniform
  | Variable
  | Op1
  | Op1Pre
  | Op2
  | Op2Pre
  | Op3Pre
  | Op4Pre
  | Select
  | Access
  deriving (Show)

data Volatility
  = Volatile
  | Pure
  deriving (Eq, Ord, Show)

-- | Rose tree. Internal AST data structure
data Tree a  = Tree
  { getElem     :: a
  , getChildren :: [Tree a]
  }


-- | Untyped Expr representation
-- Carries type information in type tag
type ExprMono = Tree (Volatility, ExprForm, CType, String)

instance Show ExprMono where
  show (Tree (_, form, _, str) xs) = case form of
    Uniform -> str
    Variable -> str
    Op1      -> mconcat ["(", str, show (xs!!0), ")"]
    Op1Pre   -> mconcat [ str, "(", show (xs!!0), ")"]
    Op2      -> mconcat ["(", show (xs !! 0), " ", str, " ", show (xs !! 1), ")"]
    Op2Pre   -> mconcat [str, "(", show (xs!!0), ", ", show (xs!!1), ")"]
    Op3Pre   -> mconcat [str, "(", show (xs!!0), ", ", show (xs!!1), ", ", show (xs!!2), ")"]
    Op4Pre   -> mconcat [str, "(", show (xs!!0), ", ", show (xs!!1), ", ", show (xs!!2), ", ", show (xs!!3), ")"]
    Select   -> mconcat ["( ", show (xs!!0), " ? ", show (xs!!1), " : ", show (xs!!2), ")"]
    Access   -> mconcat [show (xs!!0), ".", str]

-- | Light type wrapper
--
-- Note the internal type tag is not directly dependent on the actual type!
--
-- We use the ToCType typeclass to genenerate dependence from types to values
data Expr ty = Expr
  { getTypeTag :: Proxy ty
  , toMono ::  Tree (Volatility, ExprForm, CType, String)
  }

instance ToCType ty => Show (Expr ty) where
  show = show . toMono

class ToCType  ty where
  -- | Gives us dependence from typed singleton tags to untyped tags
  toCType :: Proxy ty -> CType
  -- | Singleton tag
  tag :: Proxy ty -- TODO: fill in!

-- | Unary operator.
-- Most generally typed.
op1 :: forall a b
       . (ToCType a, ToCType b)
       => String -> Expr a -> Expr b
op1 str a = Expr t (Tree (Pure, Op1, toCType t, str) [toMono a])
  where t = tag @b

-- | Unary operator.
-- Input and output values have the same type.
op1'' :: forall a
       . (ToCType a)
       => String -> Expr a -> Expr a
op1'' str a = Expr t (Tree (Pure, Op1, toCType t, str) [toMono a])
  where t = tag @a

-- | Unary operator.
-- Prefix function call style.
-- Most generally typed.
op1pre :: forall a b
          . (ToCType a, ToCType b)
          => String -> Expr a -> Expr b
op1pre str a = Expr t (Tree (Pure, Op1Pre, toCType t, str) [toMono a])
  where t = tag @b

-- | Unary operator.
-- Prefix function call style.
-- Input and output values have the same type.
op1pre'' :: forall a
          . (ToCType a)
          => String -> Expr a -> Expr a
op1pre'' str a = Expr t (Tree (Pure, Op1Pre, toCType t, str) [toMono a])
  where t = tag @a

-- | Binary operator.
-- Most generally typed.
op2 :: forall a b c
       . (ToCType a, ToCType b, ToCType c)
       => String -> Expr a -> Expr b -> Expr c
op2 str a b = Expr t (Tree (Pure, Op2, toCType t, str) [toMono a, toMono b])
  where t = tag @c

-- | Binary operator.
-- Arguments have the same type.
op2' :: forall a c
       . (ToCType a, ToCType c)
       => String -> Expr a -> Expr a -> Expr c
op2' str a b = Expr t (Tree (Pure, Op2, toCType t, str) (fmap toMono [a, b]))
  where t = tag @c

-- | Binary operator.
-- Input and output values have the same type.
op2'' :: forall a
       . (ToCType a)
       => String -> Expr a -> Expr a -> Expr a
op2'' str a b = Expr t (Tree (Pure, Op2, toCType t, str) (fmap toMono [a, b]))
  where t = tag @a


-- | Binary operator.
-- Prefix function call style.
-- Most generally typed.
op2pre :: forall a b c
          . (ToCType a, ToCType b, ToCType c)
          => String -> Expr a -> Expr b -> Expr c
op2pre str a b = Expr t (Tree (Pure, Op2Pre, toCType t, str) [toMono a, toMono b])
  where t = tag @c

-- | Binary operator.
-- Prefix function call style.
-- Arguments have the same type.
op2pre' :: forall a c
       . (ToCType a, ToCType c)
       => String -> Expr a -> Expr a -> Expr c
op2pre' str a b = Expr t (Tree (Pure, Op2Pre, toCType t, str) (fmap toMono [a, b]))
  where t = tag @c

-- | Binary operator.
-- Prefix function call style.
-- Input and output values have the same type.
op2pre'' :: forall a
       . (ToCType a)
       => String -> Expr a -> Expr a -> Expr a
op2pre'' str a b = Expr t (Tree (Pure, Op2Pre, toCType t, str) (fmap toMono [a, b]))
  where t = tag @a

-- | Ternary operator.
-- Prefix function call style.
-- Most generally typed.
op3pre :: forall a b c d
          . (ToCType a, ToCType b, ToCType c, ToCType d)
          => String -> Expr a -> Expr b -> Expr c -> Expr d
op3pre str a b c = Expr t (Tree (Pure, Op3Pre, toCType t, str) [toMono a, toMono b, toMono c])
  where t = tag @d

-- | Ternary operator.
-- Prefix function call style.
-- Arguments have the same type.
op3pre' :: forall a d
          . (ToCType a, ToCType d)
          => String -> Expr a -> Expr a -> Expr a -> Expr d
op3pre' str a b c = Expr t (Tree (Pure, Op3Pre, toCType t, str) (fmap toMono [a, b, c]))
  where t = tag @d

-- | Ternary operator.
-- Prefix function call style.
-- Input and output values have the same type.
op3pre'' :: forall a
          . (ToCType a)
          => String -> Expr a -> Expr a -> Expr a -> Expr a
op3pre'' str a b c = Expr t (Tree (Pure, Op3Pre, toCType t, str) (fmap toMono [a, b, c]))
  where t = tag @a


-- | Quaternary operator.
-- Prefix function call style.
-- Most generally typed.
op4pre :: forall a b c d e
          . (ToCType a, ToCType b, ToCType c, ToCType d, ToCType e)
          => String -> Expr a -> Expr b -> Expr c -> Expr d -> Expr e
op4pre str a b c d = Expr t (Tree (Pure, Op4Pre, toCType t, str) [toMono a, toMono b, toMono c, toMono d])
  where t = tag @e

-- | Quaternary operator.
-- Prefix function call style.
-- Arguments have the same type.
op4pre' :: forall a e
          . (ToCType a, ToCType e)
          => String -> Expr a -> Expr a -> Expr a -> Expr a -> Expr e
op4pre' str a b c d = Expr t (Tree (Pure, Op4Pre, toCType t, str) (fmap toMono [a, b, c, d]))
  where t = tag @e

-- | Quaternary operator.
-- Prefix function call style.
-- Input and output values have the same type.
op4pre'' :: forall a e
          . (ToCType a, ToCType e)
          => String -> Expr a -> Expr a -> Expr a -> Expr a -> Expr e
op4pre'' str a b c d = Expr t (Tree (Pure, Op4Pre, toCType t, str) (fmap toMono [a, b, c, d]))
  where t = tag @e




-- | Open tree type, to be used for explicit recursion with data-reify for
-- preserving sharing.
--
-- Note the second argument of the constructor is a list of Maybe b's.
-- We use Maybe's to determine whether or not a child expression gets inlined.
data TreeF a b = TreeF
  { getElemF     :: a
  , getChildrenF :: [Maybe b]
  }
  deriving (Functor)

-- | Open untyped expression representation, to be used for explicit recursion
-- with data-reify for preserving sharing.
--
-- Note the presence of a list of closed ExprMono's in the tuple.
-- We use this list to recover unshared child expressions when they need to be
-- inlined.
type ExprMonoF = TreeF (ExprForm, CType, String, [ExprMono])

-- | Returns the string representation of the nth child of an open untyped
-- expression, accounting for inlining
emfStringAt :: (Show a) => ExprMonoF a -> Int -> String
emfStringAt (TreeF (_, _, _, xs) ys)  i = zipWith fn xs ys !! i
  where
    fn x Nothing = show x
    fn _ (Just y)= show y

instance (Show a) => Show (ExprMonoF a) where
  show expr@(TreeF (form, _, str, _) _) = case form of
    Uniform  -> str
    Variable -> str
    Op1      -> mconcat ["(", str, strAt 0, ")"]
    Op1Pre   -> mconcat [ str, "(", strAt 0, ")"]
    Op2      -> mconcat ["(", strAt 0, " ", str, " ", strAt 1, ")"]
    Op2Pre   -> mconcat [str, "(", strAt 0, ", ", strAt 1, ")"]
    Op3Pre   -> mconcat [str, "(", strAt 0, ", ", strAt 1, ", ", strAt 2, ")"]
    Op4Pre   -> mconcat [str, "(", strAt 0, ", ", strAt 1, ", ", strAt 2, ", ", strAt 3, ")"]
    Select   -> mconcat ["( ", strAt 0, " ? ", strAt 1, " : ", strAt 2, ")"]
    Access   -> mconcat [strAt 0, ".", str]
    where
      strAt = emfStringAt expr

-- | Currently only inlines uniforms.
instance MuRef ExprMono where
  type DeRef ExprMono = ExprMonoF
  mapDeRef func (Tree (v, form, ty, str) xs) =
       fmap (TreeF (form, ty, str, xs)) $ g xs
    where
      -- g :: [ExprMono] -> f [Maybe u]
      g ts = for @[] ts $ \t -> traverse (func @ExprMono) $ shouldShare t

instance MuRef a => MuRef [a] where
    type DeRef [a] = Compose [] (DeRef a)

    mapDeRef
        :: forall f u. Applicative f
        => (forall b. (MuRef b, Compose [] (DeRef a) ~ DeRef b) => b -> f u)
        -> [a]
        -> f (Compose [] (DeRef a) u)
    mapDeRef f = fmap Compose . traverse go
      where
        go :: a -> f (DeRef a u)
        go = mapDeRef @a $ \(x :: b) -> f @[b] [x]

shouldShare :: ExprMono -> Maybe ExprMono
shouldShare (Tree (Volatile, _, _, _) _) = Nothing
shouldShare (Tree (_, Uniform, _, _) _)  = Nothing
shouldShare (Tree (_, _, CTVoid, _) _)   = Nothing
shouldShare expr                         = Just expr



-- | Uniform expression.
uniform :: forall a
           . ToCType a
           => String -> Expr a
uniform str = Expr t (Tree (Pure, Uniform, toCType t, str) [])
  where t = tag @a


volatile :: Expr a -> Expr a
volatile (Expr t (Tree (_, form, ty, str) xs)) = Expr t $ Tree (Volatile, form, ty, str) xs


