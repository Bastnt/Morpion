// Obtenez des informations sur F# via http://fsharp.net
// Voir le projet 'Didacticiel F#' pour obtenir de l'aide.
open System

let Dimension = 3
let WinRow = 3
let mutable PlayedActions = 0
let mutable PlayerColor = 1
let mutable staticValues : int[][] = [||]
// let mutable PrincipalVariation = []
let mutable XLastMove = 0
let mutable YLastMove = 0
let opponent c = if c = 1 then 2 else 1
// Empty color is represented as a 0

type State = 
    | Draw = 0
    | Win = 1
    | Lose = 2
    | None = 3   

let evalStaticValues dimension = 
    [| 
        [| 2; 3; 2 |]
        [| 3; 4; 3 |]
        [| 2; 3; 2 |]
    |]

let InfiniteScore = 10000000

let test = 
    [| 
        [| 0; 0; 0 |]
        [| 0; 0; 0 |]
        [| 0; 0; 0 |]
    |]



let staticEvaluation (board : int[][]) = 
    let mutable total = 0 in
        for i in 0 .. Dimension - 1 do
            for j in 0 .. Dimension - 1 do
                if board.[i].[j] = PlayerColor then
                    total <- total + staticValues.[i].[j]
                elif board.[i].[j] <> 0 then
                    total <- total - staticValues.[i].[j]
    total

let terminalState (board : int[][]) = 
    // Empty Board
    if PlayedActions = 0 then
        State.None

    //Draw state
    elif PlayedActions = (Dimension)*(Dimension) then
        State.Draw

    else
    //Other states
    let x = XLastMove in
    let y = YLastMove in
    let color = board.[x].[y] in
    let mutable a, b, row = (0,0,0) in

    // Vertical
    a <- x
    row <- 0
    while a >= 0 && board.[a].[y] = color do
        a <- a-1

    row <- (x-a)
    a <- x
    while a < Dimension && board.[a].[y] = color do
        a <- a+1
    row <- row + a - x - 1

    if(row <> WinRow) then
        // Horizontal
        b <- y
        row <- 0
        while b >= 0 && board.[x].[b] = color do
            b <- b-1

        row <- (y-b)
        b <- y
        while b < Dimension && board.[x].[b] = color do
            b <- b+1
        row <- row + b - y - 1

    if(row <> WinRow) then
        // Diagonal \
        a <- x
        b <- y
        row <- 0
        while a >= 0 && b>=0 && board.[a].[b] = color do
            a <- a-1
            b <- b-1

        row <- (x-a)
        a <- x
        b <- y
        while a < Dimension && b < Dimension do
            a <- a+1
            b <- b+1
        row <- row + a - x - 1

    if(row <> WinRow) then
        // Diagonal /
        a <- x
        b <- y
        row <- 0
        while a >= 0 && b < Dimension && board.[a].[b] = color do
            a <- a-1
            b <- b+1

        row <- (x-a)
        a <- x
        b <- y
        while a < Dimension && b >= 0 && board.[a].[b] = color do
            a <- a+1
            b <- b-1
        row <- row + a - x - 1

    if(row=WinRow) then
        if(color = PlayerColor) then 
            State.Win 
        else 
            State.Lose
    else
        State.None

let legalMoves (board : int[][]) = 
    let mutable moves = [] in
    for i in 0..Dimension-1 do
        for j in 0..Dimension-1 do
            if(board.[i].[j] = 0) then
                moves <- (i,j)::moves
    moves


let doMove (board : int[][]) (x,y) color =
    board.[x].[y] <- color
    PlayedActions <- PlayedActions + 1

let undoMove (board : int[][]) (x,y) = 
    board.[x].[y] <- 0
    PlayedActions <- PlayedActions - 1

let rec dealWithNode (board : int[][]) depth alpha beta player (movelist : byref<_>) =
    let rec dealWithChildren moves board a b =
        let mutable tmp = [] in
        if player=PlayerColor then
            match moves with
                | [] -> a
                | (x,y)::tl ->  
                    if b <= a then a
                    else 
                        (doMove board (x,y) (opponent player))
                        let v = (dealWithNode board (depth-1) a b (opponent player) &tmp) in
                        if (v > a) then
                            tmp <- (x,y)::tmp
                            (dealWithChildren tl board v b)
                        else
                            (dealWithChildren tl board a b)
        else
            match moves with
                | [] -> b
                | (x,y)::tl -> 
                    if b <= a then b 
                    else 
                        (doMove board (x,y) (opponent player))
                        let v = (dealWithNode board (depth-1) a b (opponent player) &tmp) in
                        if (v < b) then
                            tmp <- (x,y)::tmp
                            (dealWithChildren tl board a v)
                        else
                            (dealWithChildren tl board a b)
                        
    in
    match (terminalState board) with
        | State.Draw -> 0
        | State.Lose -> -InfiniteScore
        | State.Win -> InfiniteScore
        | _ -> if(depth <= 0) then (staticEvaluation board) else (dealWithChildren (legalMoves board) board alpha beta)
        

[<EntryPoint>]
let main argv = 
    staticValues <- (evalStaticValues Dimension)
    let sw = System.Diagnostics.Stopwatch()
    sw.Start()
    let mutable res = [] in
        (dealWithNode test 0 -InfiniteScore InfiniteScore 1 &res);
        printfn "Moves %A" res
    sw.Stop()
    printfn "Time: %d" sw.ElapsedMilliseconds

    (*match (terminalState test 0 1) with
        | State.None -> printfn "None"
        | State.Draw -> printfn "Draw"
        | State.Lose -> printfn "Lose"
        | State.Win -> printfn "Win"*)
    //printfn "%A" tab
    System.Console.ReadLine() |> ignore
    0 // retourne du code de sortie entier
