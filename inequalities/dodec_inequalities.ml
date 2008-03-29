
(* -------------------------------------------------------------------------- *)
(*  Util                                                                      *)
(* -------------------------------------------------------------------------- *)

let mk_times x y = mk_binop `( *. )` x y
let mk_plus x y = mk_binop `( +. )` x y 
let mk_gt x y = mk_binop `( >. )` x y 
let _0 = `&0`
let _1 = `&1`
let m_1 = `-- &1`
let list_disj l = end_itlist (curry mk_disj) l

(* -------------------------------------------------------------------------- *)
(*  Definitions                                                               *)
(* -------------------------------------------------------------------------- *)

(* 
 * t = 1.25841
 * 2t = 2.51682
 * (2t)^2 = 6.3343829124
 *)
let dodecTrunc = kepler_def `dodecTrunc = #1.25841`
let twoDodecTrunc = kepler_def `twoDodecTrunc = #2.51682`
let twoDodecTruncSq = kepler_def `twoDodecTruncSq = #6.3343829124`
let dodecSlope = kepler_def `dodecSlope = #0.42755`

let volume_x = kepler_def(`volume_x x1 x2 x3 x4 x5 x6 =
        let del = sqrt (delta_x x1 x2 x3 x4 x5 x6) in
        let u126 = ups_x x1 x2 x6 in
        let u135 = ups_x x1 x3 x5 in
        let u234 = ups_x x2 x3 x4 in
        let vol = ((&1)/((&48)*del))*
                ((x1*(x2+x6-x1)+x2*(x1+x6-x2))*(chi_x x4 x5 x3 x1 x2 x6)/u126
               +(x2*(x3+x4-x2)+x3*(--x3+x4+x2))*(chi_x x6 x5 x1 x3 x2 x4)/u234
               +(x1*(--x1+x3+x5)+x3*(x1-x3+x5))*(chi_x x4 x6 x2 x1 x3 x5)/u135) in
          vol`)

let truncated_volume_x = kepler_def(`truncated_volume_x x1 x2 x3 x4 x5 x6 =
        let vor = vort_x x1 x2 x3 x4 x5 x6 dodecTrunc in
        let sol = sol_x x1 x2 x3 x4 x5 x6 in
          (vor - (&4 / &3) * sol) / (--(&4) * doct)`)

let omega_x = kepler_def( `omega_x x1 x2 x3 x4 x5 x6 =
    if eta_x x1 x2 x6 < sqrt2 /\ 
       eta_x x1 x3 x5 < sqrt2 /\ 
       eta_x x2 x3 x4 < sqrt2 /\
       eta_x x4 x5 x6 < sqrt2 
    then 
      volume_x x1 x2 x3 x4 x5 x6
    else
      truncated_volume_x x1 x2 x3 x4 x5 x6`)

let squander_x = kepler_def( `squander_x x1 x2 x3 x4 x5 x6 =
    omega_x x1 x2 x3 x4 x5 x6 - dodecSlope * sol_x x1 x2 x3 x4 x5 x6`)
                          
(* -------------------------------------------------------------------------- *)
(*  Tetrahedra                                                                *)
(* -------------------------------------------------------------------------- *)

let basicTet =
  let bnds = `ineq 
    [(&4, x1, twoDodecTruncSq);
     (&4, x2, twoDodecTruncSq);
     (&4, x3, twoDodecTruncSq);
     (&4, x4, twoDodecTruncSq);
     (&4, x5, twoDodecTruncSq);
     (&4, x6, twoDodecTruncSq)]` in
  let mk_p (vol,sol,dih,y1,y2,y3,y4,y5,y6,const) = 
    let vt = mk_times vol `omega_x x1 x2 x3 x4 x5 x6` in
    let st = mk_times sol `sol_x x1 x2 x3 x4 x5 x6` in
    let dt = mk_times dih `dih_x x1 x2 x3 x4 x5 x6` in
    let y1t = mk_times y1 `sqrt x1` in
    let y2t = mk_times y2 `sqrt x2` in
    let y3t = mk_times y3 `sqrt x3` in
    let y4t = mk_times y4 `sqrt x4` in
    let y5t = mk_times y5 `sqrt x5` in
    let y6t = mk_times y6 `sqrt x6` in
    let lhs = end_itlist mk_plus [vt;st;dt;y1t;y2t;y3t;y4t;y5t;y6t;const] in
      mk_gt lhs _0 in
    fun args -> all_forall (mk_comb(bnds,mk_p args))

let D_746258463 = basicTet(_1,_0,_0,_0,_0,_0,_0,_0,_0,`-- #0.202804`)
let D_700227380 = basicTet(_0,_1,_0,_0,_0,_0,_0,_0,_0,`-- #0.315696`)
let D_221244893 = basicTet(_0,m_1,_0,_0,_0,_0,_0,_0,_0,`#1.051232`)
let D_780513950 = basicTet(_0,_0,_1,_0,_0,_0,_0,_0,_0,`-- #0.856147`)
let D_780513950 = basicTet(_0,_0,m_1,_0,_0,_0,_0,_0,_0,`#1.886730`)
let D_492968739 = basicTet(_1,`-- #0.42755`,_0,_0,_0,_0,_0,_0,_0,_0)
let D_618710366 = basicTet(_1,`-- #0.68`,`#1.88718`,_0,_0,_0,_0,_0,_0,`-- #1.54551`)
let D_213941777 = basicTet(_1,`-- #0.68`,`#0.90746`,_0,_0,_0,_0,_0,_0,`-- #0.706725`)
let D_850728205 = basicTet(_1,`-- #0.68`,`#0.46654`,_0,_0,_0,_0,_0,_0,`-- #0.329233`)
let D_267095395 = basicTet(_1,`-- #0.55889`,_0,_0,_0,_0,_0,_0,_0,`#0.0736486`)
let D_627414008 = basicTet(_1,`-- #0.63214`,_0,_0,_0,_0,_0,_0,_0,`#0.13034`)
let D_589410050 = basicTet(_1,`-- #0.73256`,_0,_0,_0,_0,_0,_0,_0,`#0.23591`)
let D_259148858 = basicTet(_1,`-- #0.89346`,_0,_0,_0,_0,_0,_0,_0,`#0.40505`)
let D_823854484 = basicTet(_1,`-- #0.3`,`-- #0.5734`,_0,_0,_0,_0,_0,_0,`#0.978221`)
let D_106961488 = basicTet(_1,`-- #0.3`,`-- #0.03668`,_0,_0,_0,_0,_0,_0,`-- #0.024767`)
let D_226458649 = basicTet(_1,`-- #0.3`,`#0.04165`,_0,_0,_0,_0,_0,_0,`-- #0.121199`)
let D_226458649 = basicTet(_1,`-- #0.3`,`#0.1234`,_0,_0,_0,_0,_0,_0,`-- #0.209279`)
let D_502816365 = basicTet(_1,`-- #0.42755`,`-- #0.11509`,_0,_0,_0,_0,_0,_0,`#0.171859`)
let D_198727973 = basicTet(_1,`-- #0.42755`,`-- #0.04078`,_0,_0,_0,_0,_0,_0,`#0.050713`)
let D_472172699 = basicTet(_1,`-- #0.42755`,`#0.11031`,_0,_0,_0,_0,_0,_0,`-- #0.135633`)
let D_425965096 = basicTet(_1,`-- #0.42755`,`#0.13091`,_0,_0,_0,_0,_0,_0,`-- #0.157363`)
let D_239807732 = basicTet(_1,`-- #0.55792`,`-- #0.21394`,_0,_0,_0,_0,_0,_0,`#0.417998`)
let D_394858204 = basicTet(_1,`-- #0.55792`,`-- #0.0068`,_0,_0,_0,_0,_0,_0,`#0.081902`)
let D_944559581 = basicTet(_1,`-- #0.55792`,`#0.0184`,_0,_0,_0,_0,_0,_0,`#0.051224`)
let D_505826407 = basicTet(_1,`-- #0.55792`,`#0.24335`,_0,_0,_0,_0,_0,_0,`-- #0.193993`)
let D_582219051 = basicTet(_1,`-- #0.68`,`-- #0.30651`,_0,_0,_0,_0,_0,_0,`#0.648496`)
let D_822532833 = basicTet(_1,`-- #0.68`,`-- #0.06965`,_0,_0,_0,_0,_0,_0,`#0.278`)
let D_463829610 = basicTet(_1,`-- #0.68`,`#0.0172`,_0,_0,_0,_0,_0,_0,`#0.15662`)
let D_576761286 = basicTet(_1,`-- #0.68`,`#0.41812`,_0,_0,_0,_0,_0,_0,`-- #0.287778`)
let D_617344240 = basicTet(_1,`-- #0.64934`,_0,_0,_0,_0,_0,_0,_0,`#0.14843`)
let D_198434292 = basicTet(_1,`-- #0.6196`,_0,_0,_0,_0,_0,_0,_0,`#0.118`)
let D_594973443 = basicTet(_1,`-- #0.58402`,_0,_0,_0,_0,_0,_0,_0,`#0.090290`)
let D_502069976 = basicTet(_1,`-- #0.25181`,_0,_0,_0,_0,_0,_0,_0,`-- #0.096509`)
let D_839413472 = basicTet(_1,`-- #0.00909`,_0,_0,_0,_0,_0,_0,_0,`-- #0.199559`)
let D_839413472 = basicTet(_1,`#0.93877`,_0,_0,_0,_0,_0,_0,_0,`-- #0.537892`)
let D_839413472 = basicTet(_1,`#0.93877`,`-- #0.20211`,_0,_0,_0,_0,_0,_0,`-- #0.27313`)
let D_605554103 = basicTet(_1,`#0.93877`,`#0.63517`,_0,_0,_0,_0,_0,_0,`-- #1.20578`)
let D_565689141 = basicTet(_1,`#1.93877`,_0,_0,_0,_0,_0,_0,_0,`-- #0.854804`)
let D_126113068 = basicTet(_1,`#1.93877`,`-- #0.20211`,_0,_0,_0,_0,_0,_0,`-- #0.621886`)
let D_126113068 = basicTet(_1,`#1.93877`,`#0.63517`,_0,_0,_0,_0,_0,_0,`-- #1.57648`)
let D_912620251 = basicTet(_1,`-- #0.42775`,_0,_0,_0,_0,_0,_0,_0,`#0.000111`)
let D_379383832 = basicTet(_1,`-- #0.55792`,_0,_0,_0,_0,_0,_0,_0,`#0.073037`)
let D_680360097 = basicTet(_1,_0,`-- #0.07853`,_0,_0,_0,_0,_0,_0,`-- #0.08865`)
let D_936587998 = basicTet(_1,_0,`-- #0.00339`,_0,_0,_0,_0,_0,_0,`-- #0.198693`)
let D_665647024 = basicTet(_1,_0,`#0.18199`,_0,_0,_0,_0,_0,_0,`-- #0.396670`)
let D_489032744 = basicTet(_1,`-- #0.42755`,`-- #0.2`,_0,_0,_0,_0,_0,_0,`#0.332061`)
let D_803032877 = basicTet(_1,`-- #0.3`,`-- #0.36373`,_0,_0,_0,_0,_0,_0,`#0.58263`)
let D_351830373 = basicTet(_1,`-- #0.3`,`#0.20583`,_0,_0,_0,_0,_0,_0,`-- #0.279851`)
let D_768384016 = basicTet(_1,`-- #0.3`,`#0.40035`,_0,_0,_0,_0,_0,_0,`-- #0.446389`)
let D_171380710 = basicTet(_1,`-- #0.3`,`#0.83259`,_0,_0,_0,_0,_0,_0,`-- #0.816450`)
let D_834346374 = basicTet(_1,`-- #0.42755`,`-- #0.51838`,_0,_0,_0,_0,_0,_0,`#0.932759`)
let D_834346374 = basicTet(_1,`-- #0.42755`,`#0.29344`,_0,_0,_0,_0,_0,_0,`-- #0.296513`)
let D_700440189 = basicTet(_1,`-- #0.42755`,`#0.57056`,_0,_0,_0,_0,_0,_0,`-- #0.533768`)
let D_965725558 = basicTet(_1,`-- #0.42755`,`#1.18656`,_0,_0,_0,_0,_0,_0,`-- #1.06115`)
let D_809125440 = basicTet(_1,`-- #0.55792`,`-- #0.67644`,_0,_0,_0,_0,_0,_0,`#1.29062`)
let D_937646279 = basicTet(_1,`-- #0.55792`,`#0.38278`,_0,_0,_0,_0,_0,_0,`-- #0.313365`)
let D_264424411 = basicTet(_1,`-- #0.55792`,`#0.74454`,_0,_0,_0,_0,_0,_0,`-- #0.623085`)
let D_238418427 = basicTet(_1,`-- #0.55792`,`#1.54837`,_0,_0,_0,_0,_0,_0,`-- #1.31128`)
let D_864208717 = basicTet(_1,`-- #0.68`,`-- #0.82445`,_0,_0,_0,_0,_0,_0,`#1.62571`)

let D_520553364 = basicTet
  (_0,_1,_0,`#0.245`,`#0.245`,`#0.245`,
   `-- #0.063`,`-- #0.063`,`-- #0.063`,`-- #1.643285`)

let D_520553364 = basicTet
  (_0,_1,_0,`#0.3798`,`#0.3798`,`#0.3798`,
   `-- #0.198`,`-- #0.198`,`-- #0.198`,`-- #1.642085`)

let D_520123967 = basicTet
  (_0,m_1,_0,`-- #0.151`,`-- #0.151`,`-- #0.151`,
   `#0.323`,`#0.323`,`#0.323`,`-- #0.480715`)

let D_894897557 = basicTet
  (_1,`-- #0.42755`,_0,`-- #0.0392`,`-- #0.0392`,`-- #0.0392`,
   `-- #0.0101`,`-- #0.0101`,`-- #0.0101`,`#0.2958`)

let D_545267000 = basicTet
  (_1,_0,_0,`#0.107`,`#0.107`,`#0.107`,
   `-- #0.116`,`-- #0.116`,`-- #0.116`,`-- #0.181702`)

let D_846874024 = basicTet
  (_1,_0,_0,`#0.0623`,`#0.0623`,`#0.0623`,
   `-- #0.0722`,`-- #0.0722`,`-- #0.0722`,`-- #0.176302`)

let D_358961535 = basicTet
  (_0,_0,_1,`-- #0.237`,`#0.372`,`#0.372`,
   `-- #0.708`,`#0.372`,`#0.372`,`-- #2.31695`)

let D_900774652 = basicTet
  (_0,_0,_1,`-- #0.237`,`#0.363`,`#0.363`,
   `-- #0.688`,`#0.363`,`#0.363`,`-- #2.28495`)

let D_582976537 = basicTet
  (_0,_0,m_1,`#0.505`,`-- #0.152`,`-- #0.152`,
   `#0.766`,`-- #0.152`,`-- #0.152`,`-- #0.09505`)

(* -------------------------------------------------------------------------- *)
(*  Quadrilaterals                                                            *)
(* -------------------------------------------------------------------------- *)

let basicQuad =
  let bnds = `ineq 
    [(&4, x1, twoDodecTruncSq);
     (&4, x2, twoDodecTruncSq);
     (&4, x3, twoDodecTruncSq);
     (twoDodecTruncSq, x4, square (#3.2));
     (&4, x5, twoDodecTruncSq);
     (&4, x6, twoDodecTruncSq);
     (&4, x7, twoDodecTruncSq);
     (&4, x8, twoDodecTruncSq);
     (&4, x9, twoDodecTruncSq)]` in
  let mk_times x y = mk_binop `( *. )` x y in
  let mk_plus x y = mk_binop `( +. )` x y in
  let mk_gt x y = mk_binop `( >. )` x y in
  let constr1 = `delta_x x1 x2 x3 x4 x5 x6 < &0` in
  let constr2 = `delta_x x7 x2 x3 x4 x8 x9 < &0` in
  let constr3 = `cross_diag_x x1 x2 x3 x4 x5 x6 x7 x8 x9 < twoDodecTrunc` in
  let mk_p (vol,sol,dih,const) = 
    let vt = mk_times vol `omega_x x1 x2 x3 x4 x5 x6 + omega_x x7 x2 x3 x4 x8 x9` in
    let st = mk_times sol `sol_x x1 x2 x3 x4 x5 x6 + sol_x x7 x2 x3 x4 x8 x9` in
    let dt = mk_times dih `dih_x x1 x2 x3 x4 x5 x6` in
    let rhs = end_itlist  mk_plus [vt;st;dt;const] in
      mk_gt rhs _0 in
    fun args -> 
      all_forall (mk_comb(bnds,list_disj [mk_p args;constr1;constr2;constr3]))

let D_317815056 = basicQuad (_1,_0,_0,`-- #0.455149`)
let D_544371408 = basicQuad (_0,_1,_0,`-- #0.731937`)
let D_997664158 = basicQuad (_0,m_1,_0,`#2.85860`)
let D_364076929 = basicQuad (_0,_0,_1,`-- #1.15242`)
let D_718802901 = basicQuad (_0,_0,m_1,`#3.25887`)
let D_559041824 = basicQuad (_1,`-- #0.42755`,_0,`-- #0.031350`)
let D_112735258 = basicQuad (_1,`-- #0.42775`,`-- #0.15098`,`-- #0.3670`)
let D_320227884 = basicQuad (_1,`-- #0.42775`,`-- #0.09098`,`-- #0.1737`)
let D_119714323 = basicQuad (_1,`-- #0.42775`,`-- #0.00000`,`#0.0310`)
let D_139428969 = basicQuad (_1,`-- #0.42775`,`#0.18519`,`#0.3183`)
let D_871929542 = basicQuad (_1,`-- #0.42775`,`#0.20622`,`#0.3438`)
let D_102795888 = basicQuad (_1,`-- #0.55792`,`-- #0.30124`,`-- #1.0173`)
let D_348514421 = basicQuad (_1,`-- #0.55792`,`-- #0.02921`,`-- #0.2101`)
let D_224290658 = basicQuad (_1,`-- #0.55792`,`-- #0.00000`,`-- #0.1393`)
let D_399869238 = basicQuad (_1,`-- #0.55792`,`#0.05947`,`-- #0.0470`)
let D_961740289 = basicQuad (_1,`-- #0.55792`,`#0.39938`,`#0.4305`)
let D_884691741 = basicQuad (_1,`-- #0.55792`,`#2.50210`,`#2.8976`)
let D_575770853 = basicQuad (_1,`-- #0.68000`,`-- #0.44194`,`-- #1.6264`)
let D_111528497 = basicQuad (_1,`-- #0.68000`,`-- #0.10957`,`-- #0.6753`)
let D_301601713 = basicQuad (_1,`-- #0.68000`,`-- #0.00000`,`-- #0.4029`)
let D_167315158 = basicQuad (_1,`-- #0.68000`,`#0.86096`,`#0.8262`)
let D_696582857 = basicQuad (_1,`-- #0.68000`,`#2.44439`,`#2.7002`)
let D_326971673 = basicQuad (_1,`-- #0.30000`,`-- #0.12596`,`-- #0.1279`)
let D_142619096 = basicQuad (_1,`-- #0.30000`,`-- #0.02576`,`#0.1320`)
let D_199465624 = basicQuad (_1,`-- #0.30000`,`#0.00000`,`#0.1945`)
let D_820793486 = basicQuad (_1,`-- #0.30000`,`#0.03700`,`#0.2480`)
let D_615384371 = basicQuad (_1,`-- #0.30000`,`#0.22476`,`#0.5111`)
let D_311116938 = basicQuad (_1,`-- #0.30000`,`#2.31852`,`#2.9625`)
let D_655018058 = basicQuad (_1,_0,`-- #0.23227`,`-- #0.1042`)
let D_608521308 = basicQuad (_1,_0,`#0.07448`,`#0.5591`)
let D_623607650 = basicQuad (_1,_0,`#0.22019`,`#0.7627`)
let D_858671466 = basicQuad (_1,_0,`#0.80927`,`#1.5048`)
let D_381451501 = basicQuad (_1,_0,`#5.84380`,`#7.3468`)

