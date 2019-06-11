{-# OPTIONS_GHC -Wall #-}

module Data.C.Program where

import Data.Reify
import System.IO.Unsafe
import Data.C.Expr
import Data.C.Types
import Control.Monad.Writer
import Data.Traversable

newtype Id = Id Int
instance Show Id where
  show (Id h) = "_" <> show h

-- | Statement internal representation
--
-- We tag a Statement with a Unique ID and its corresponding untyped expression
data Statement
  = NewAssign (Unique, ExprMonoF Unique)

getExpr :: Statement -> ExprMonoF Unique
getExpr (NewAssign (_, expr)) = expr

instance Show Statement where
  show (NewAssign (_, expr@(TreeF (_, CTVoid, _, _) _))) =
    mconcat
      [ show $ fmap Id expr
      , ";"
      ]
  show (NewAssign (i, expr@(TreeF (_, ty, _, _) _))) =
    mconcat
      [ show ty
      , " "
      , show $ Id i
      , " = "
      , show $ fmap Id expr
      , ";"
      ]

-- | GLSL Function internal representation
--
-- A Function is composed of Statements.
newtype Function = Function [Statement]
instance Show Function where
  show (Function xs) =
    unlines
      [ "void main() {"
      , assignments
      , "    gl_FragColor = _1;"
      , "}"
      ]
    where
      assignments = unlines $ ("\n\t" <>) . show <$> reverse xs


-- | Returns a program given an expression in closed untyped form
monoToProgram :: ExprMono -> Function
monoToProgram v = unsafePerformIO $ do
  Graph nodes _ <- reifyGraph v
  return . Function $ NewAssign <$> nodes

-- | A GLSL program. Currently synonym for Function.
type Program = Function

-- | Helper function from a Vec4 to A GLSL Program, with sharing.
toProgram :: CInt -> Program
toProgram = monoToProgram . toMono

toJonk :: [SomeCType] -> Program
toJonk cs = Function . unsafePerformIO $ do
  z <- for cs $ \(SomeCType c) -> do
    Graph nodes _ <- reifyGraph $ toMono c
    pure $ NewAssign <$> nodes
  pure $ join z


-- $> print $ toJonk $ let foo = 6; bar = foo + 5 in [SomeCType @'CTInt $ foo + bar, SomeCType $ showNumber $ foo + bar + bar]


