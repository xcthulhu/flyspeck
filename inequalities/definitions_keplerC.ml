(* Start from the beginning of the text of Unabridged Proof
   of the Kepler Conjecture. Version Nov 26, 2003 *)

(*
let mk_vec3 = kepler_def `mk_vec3 y1 y2 y3 = 
  (\i. if (i=0) then y1 else if (i=1) then y2 else if (i=2) then y3
   else (&.0))`;;
*)

let mk_lattice = kepler_def `mk_lattice v1 v2 v3 =
  { z | ?m1 m2 m3. (z = (dest_int m1) *# v1 + (dest_int m2) *# v2 +
      (dest_int m3) *# v3 ) }`;;

let fcc_packing = kepler_def `fcc_packing = 
  let v1 = mk_vec3 (&.2) (&.0) (&.0) in
  let v2 = mk_vec3 (&.1) (sqrt (&.3)) (&.0) in
  let v3 = mk_vec3 (&.1) ((sqrt (&.3))/(&.3)) ((&.2)*sqrt(&.6)/(&.3)) in
  mk_lattice v1 v2 v3`;;


let hcp_packing = kepler_def `hcp_packing = 
  let v1 = mk_vec3 (&.2) (&.0) (&.0) in
  let v2 = mk_vec3 (&.1) (sqrt (&.3)) (&.0) in
  let v3 = mk_vec3 (&.1) ((sqrt (&.3))/(&.3)) ((&.2)*sqrt(&.6)/(&.3)) in
  let v4 = mk_vec3 (&.0) (&.0) ((&.4)*sqrt(&.6)/(&.3)) in
  let L = mk_lattice v1 v2 v4 in
  L UNION (set_translate v3 L)`;;

(* B(x,r).  Changed from closed in text.  *)
let ball3 = kepler_def `ball3 x r = 
      open_ball (euclid 3,d_euclid) x r`;;

(* B(x,r,Lambda) *)
let ball3_lambda = kepler_def 
  `ball3_lambda x r Lambda =
   ( ball3 x r) INTER (UNIONS(IMAGE (\v. ball3 v (&.1)) Lambda))`;;

(* delta(x,r,Lambda) *)
let delta_finite = kepler_def
  `delta_finite x r Lambda = 
    (vol 3 (ball3_lambda x r Lambda))/(vol 3 (ball3 x r))`;;

(* Lambda(x,r) *)
let truncated_packing = kepler_def
  `truncated_packing x r Lambda = 
   Lambda INTER (ball3_lambda x r Lambda)`;;

(* Omega(v,Lambda) *)
let closed_voronoi_cell = kepler_def
  `closed_voronoi_cell v Lambda = 
    {x | euclid 3 x /\ 
       (!w. (Lambda w) ==> (d_euclid x v <= d_euclid x w)) }`;;

let open_voronoi_cell = kepler_def
  `open_voronoi_cell v Lambda = 
    {x | euclid 3 x /\ 
       (!w. (Lambda w) /\ ~(w = v) ==> (d_euclid x v <. d_euclid x w)) }`;;
let fcc_voronoi_volume = 
  `!v. (fcc_packing v) ==>
     (vol 3 (open_voronoi_cell v fcc_packing) = (sqrt(&.32)))`;;

let hcp_voronoi_volume = 
  `!v. (hcp_packing v) ==>
     (vol 3 (open_voronoi_cell v hcp_packing) = sqrt(&.32))`;;

let negligible = kepler_def 
  `negligible Lambda A = ?C. (!r x. (euclid 3 x) /\ (&.1 <=. r) ==>
     ITSET (\v acc. A(v) +. acc) (truncated_packing x r Lambda) (&.0)
     <=. (C * r * r))`;;

let fcc_compatible = kepler_def
  `fcc_compatible Lambda A = !v. (Lambda v) ==> 
    (sqrt(&.32) <=. (vol 3 (open_voronoi_cell v Lambda)) + (A v))`;;

let compatible_density = 
  `!Lambda. 
    (saturated_packing Lambda) /\
    (Lambda SUBSET (euclid 3)) /\
    (?A. (fcc_compatible Lambda A) /\ (negligible Lambda A)) ==> 
    (?C. (! x r. (euclid 3 x) /\ (&.1 <=. r) ==>
        (delta_finite x r Lambda <=. pi/(sqrt (&.18)) + C/r)))`;;

let kepler_conjecture = 
  `!Lambda. ?C.  !x r. 
     (Lambda SUBSET (euclid 3)) /\
     (saturated_packing Lambda) /\ 
     (euclid 3 x) /\ (&.1 <=. r) ==>
      ( (delta_finite x r Lambda <=. pi/(sqrt (&. 18)) + C/r))`;;

(* skipped some top-down material that doesn't make sense at this point *)

let tetrahedron_vol = kepler_def 
  `tetrahedron_vol = 
  let v0 = mk_vec3 (&.0) (&.0) (&.0) in 
  let v1 = mk_vec3 (&.2) (&.0) (&.0) in
  let v2 = mk_vec3 (&.1) (sqrt (&.3)) (&.0) in
  let v3 = mk_vec3 (&.1) ((sqrt (&.3))/(&.3)) ((&.2)*sqrt(&.6)/(&.3)) in
  vol 3 (convex_hull {v0,v1,v2,v3})`;;

let tetrahedron_ball_vol = kepler_def 
  `tetrahedron_ball_vol = 
  let v0 = mk_vec3 (&.0) (&.0) (&.0) in 
  let v1 = mk_vec3 (&.2) (&.0) (&.0) in
  let v2 = mk_vec3 (&.1) (sqrt (&.3)) (&.0) in
  let v3 = mk_vec3 (&.1) ((sqrt (&.3))/(&.3)) ((&.2)*sqrt(&.6)/(&.3)) in
  vol 3 (convex_hull {v0,v1,v2,v3} INTER 
      (UNIONS (IMAGE (\v. ball3 v (&.1)) {v0,v1,v2,v3})))`;;

let dtet_lemma = 
  `dtet = (tetrahedron_ball_vol/tetrahedron_vol)`;;

let octahedron_vol = kepler_def 
  `octahedron_vol = 
  let v0 = mk_vec3 (&.0) (&.0) (-- (sqrt(&.2))) in 
  let v1 = mk_vec3 (&.1) (&.1) (&.0) in
  let v2 = mk_vec3 (&.1) (--(&.1)) (&.0) in
  let v3 = mk_vec3 (-- (&.1)) (&.1) (&.0) in
  let v4 = mk_vec3 (-- (&.1)) (-- (&.1)) (&.0) in
  let v5 = mk_vec3 (&.0) (&.0) ( (sqrt(&.2))) in 
  let S = {v0,v1,v2,v3,v4,v5} in
  vol 3 (convex_hull S)`;;

let octahedron_ball_vol = kepler_def 
  `octahedron_ball_vol = 
  let v0 = mk_vec3 (&.0) (&.0) (-- (sqrt(&.2))) in 
  let v1 = mk_vec3 (&.1) (&.1) (&.0) in
  let v2 = mk_vec3 (&.1) (--(&.1)) (&.0) in
  let v3 = mk_vec3 (-- (&.1)) (&.1) (&.0) in
  let v4 = mk_vec3 (-- (&.1)) (-- (&.1)) (&.0) in
  let v5 = mk_vec3 (&.0) (&.0) ( (sqrt(&.2))) in 
  let S = {v0,v1,v2,v3,v4,v5} in
  vol 3  (convex_hull S INTER 
      (UNIONS (IMAGE (\v. ball3 v (&.1)) S)))`;;

let doct_lemma = 
  `doct = (octahedron_ball_vol/octahedron_vol)`;;

let dtet_doct = 
  `pi/(sqrt(&.18)) = dtet/(&.3) + (&.2)*doct/(&.3)`;;

let pt_dtet = 
  `pt = (sqrt(&.2))*dtet - pi/(&.3)`;;

let pt_doct = 
  `pt = (-- (&.2))*(sqrt(&.2)*doct - pi/(&.3))`;;

(* Construction of the Q-system *)

let quasi_regular_triangle = kepler_def
  `quasi_regular_triangle Lambda S = (S HAS_SIZE 3) /\
   (S SUBSET Lambda) /\
   (!v w. (S v ) /\ (S w) ==> (d_euclid w v <= two_t0))`;;

let simplex = kepler_def
  `simplex Lambda (S:(num->real)->bool) = (S HAS_SIZE 4) /\
    (S SUBSET Lambda)`;;

let quasi_regular_tet = kepler_def
  `quasi_regular_tet Lambda S = (simplex Lambda S) /\ 
    (!v w. (S v) /\ (S w) ==> (d_euclid w v <= two_t0))`;;

let quarter = kepler_def
  `quarter Lambda S = (simplex Lambda S) /\
    (?v w. (S v) /\ (S w) /\ (two_t0 <=. d_euclid w v) /\
       (d_euclid w v <=. sqrt8) /\ 
       (!x y. (S x) /\ (S y) /\ (two_t0 <. d_euclid x y) 
          ==> ({x,y} = {v,w})))`;;

let strict_quarter = kepler_def
  `strict_quarter Lambda S = (simplex Lambda S) /\
    (?v w. (S v) /\ (S w) /\ (two_t0 <. d_euclid w v) /\
       (d_euclid w v <. sqrt8) /\ 
       (!x y. (S x) /\ (S y) /\ (two_t0 <. d_euclid x y) 
          ==> ({x,y} = {v,w})))`;;

let diagonal = kepler_def
  `diagonal S d = (d SUBSET S) /\ 
     (?d1 d2. (d = {d1,d2}) /\ 
         (!u v. (S u) /\ (S v) ==>(d_euclid u v <=. d_euclid d1 d2)))`;;


let two_to_2t0 = kepler_def(`two_to_2t0 x =
        ((&2)<= x) /\ (x <= two_t0)`);;

let twot0_to_sqrt8 = kepler_def(`twot0_to_sqrt8 x = 
        (two_t0 <= x) /\ (x <= sqrt8)`);;

let is_qrtet_y = kepler_def(`is_qrtet_y y1 y2 y3 y4 y5 y6 =
        (two_to_2t0 y1) /\
        (two_to_2t0 y2) /\
        (two_to_2t0 y3) /\
        (two_to_2t0 y4) /\
        (two_to_2t0 y5) /\
        (two_to_2t0 y6)`);;

let s_to_8 = kepler_def(`s_to_8 x =
        ((#6.3001 <= x) /\ (x <= (&.8)))`);;

let four_to_s = kepler_def(`four_to_s x =
        ((&.4)<= x) /\ (x <= #6.3001)`);;

let is_qrtet_x = kepler_def(`is_qrtet_x x1 x2 x3 x4 x5 x6 =
        (four_to_s x1) /\
        (four_to_s x2) /\
        (four_to_s x3) /\
        (four_to_s x4) /\
        (four_to_s x5) /\
        (four_to_s x6)`);;

let is_upright_quarter_y = kepler_def(`is_upright_quarter_y y1 y2 y3 y4 y5
y6 = 
        (twot0_to_sqrt8 y1) /\
        (two_to_2t0 y2) /\
        (two_to_2t0 y3) /\
        (two_to_2t0 y4) /\
        (two_to_2t0 y5) /\
        (two_to_2t0 y6)`);;

let is_upright_quarter_v = kepler_def(`is_upright_quarter_v v0 v1 v2 v3=
        is_upright_quarter_y
        (d_euclid v0 v1) (d_euclid v0 v2) (d_euclid v0 v3)
        (d_euclid v2 v3) (d_euclid v1 v3) (d_euclid v2 v3)`);;

let dih_v = kepler_def(`dih_v v0 v1 v2 v3 =
        dih_y (d_euclid v0 v1) (d_euclid v0 v2) (d_euclid v0 v3)
        (d_euclid v2 v3) (d_euclid v1 v3) (d_euclid v1 v2)`);;

(* an equivalent definition of dih, except it works better in the
degenerate case of delta = 0 in distinguishing between angle 0 and pi *)

let dih_alt_x = kepler_def(`dih_alt_x x1 x2 x3 x4 x5 x6 =
        acs ((delta_x4 x1 x2 x3 x4 x5 x6)/
                sqrt((u_x x1 x2 x6)*(u_x x1 x3 x5)))`);;

let dih_alt_y = kepler_def(`dih_alt_y y1 y2 y3 y4 y5 y6 = 
        let (x1,x2,x3,x4,x5,x6)=(y1*y1,y2*y2,y3*y3,y4*y4,y5*y5,y6*y6) in
        dih_alt_x x1 x2 x3 x4 x5 x6`);;

let dih_alt_v = kepler_def(`dih_alt_v v0 v1 v2 v3 =
        dih_alt_y (d_euclid v0 v1) (d_euclid v0 v2) (d_euclid v0 v3)
        (d_euclid v2 v3) (d_euclid v1 v3) (d_euclid v1 v2)`);;

(* oriented dihedral takes a value from 0 to < 2pi *)
let or_dih_v = kepler_def(`or_dih_v v0 v1 v2 v3 =
        let w1 =  v1 - v0 in
        let w2 =  v2 - v0 in
        let w3 =  v3 - v0 in
        let triple = triple_product w1 w2 w3 in
        if (triple > (&0)) then (dih_v v0 v1 v2 v3)
        else if (triple < (&0)) then ((&2)*pi - (dih_v v0 v1 v2 v3))
        else (dih_alt_v v0 v1 v2 v3)`);;

(* traverse the boundary v1 v2 v3 v4, with the region to the left
        as you circle around *)

let solid_area_quad_v = kepler_def(`solid_area_quad_v v0 v1 v2 v3 v4 =
        (or_dih_v v0 v1 v2 v4) +
        (or_dih_v v0 v2 v3 v1) +
        (or_dih_v v0 v3 v4 v2) +
        (or_dih_v v0 v4 v1 v3) - (&2)*pi`);;

let is_quad_cluster_v = kepler_def(`is_quad_cluster_v v0 v1 v2 v3 v4 =
        let y1 = d_euclid v0  v1 in
        let y2 = d_euclid v0  v2 in
        let y3 = d_euclid v0  v3 in
        let y4 = d_euclid v0  v4 in
        let e12 = d_euclid v1 v2 in
        let e23 = d_euclid v2 v3 in
        let e34 = d_euclid v3 v4 in
        let e41 = d_euclid v4 v1 in
        let d13 = d_euclid v1 v3 in
        let d24 = d_euclid v2 v4 in
        (two_to_2t0 y1) /\
        (two_to_2t0 y2) /\
        (two_to_2t0 y3) /\
        (two_to_2t0 y4) /\
        (two_to_2t0 e12) /\
        (two_to_2t0 e23) /\
        (two_to_2t0 e34) /\
        (two_to_2t0 e41) /\
        (two_t0 <= d13) /\
        (two_t0 <= d24) `);;

let upright_oct_v = kepler_def(`upright_oct_v v0 w v1 v2 v3 v4 =
        ((is_upright_quarter_v v0 w v1 v2) /\
        (is_upright_quarter_v v0 w v2 v3) /\
        (is_upright_quarter_v v0 w v3 v4) /\
        (is_upright_quarter_v v0 w v4 v1))`);;

let is_pairflat13 = kepler_def(`is_pairflat13 v0 v1 v2 v3 v4 =
        (is_quad_cluster_v v0 v1  v2 v3 v4) /\
        ((d_euclid v1 v3) <= sqrt8)`);;

let is_pairflat24 = kepler_def(`is_pairflat24 v0 v1 v2 v3 v4 =
        (is_quad_cluster_v v0 v1  v2 v3 v4) /\
        ((d_euclid v2 v4) <= sqrt8)`);;

(* we make w lie in the open cone at v0 spanned by (v[i]-v0) *)
let is_enclosed = kepler_def
  `is_enclosed w v0 v1 v2 v3 =
     (?a0 a1 a2 a3. ((w = a0 *# v0 + a1 *# v1 + a2 *# v2 + a3 *# v3)
        /\
                ((&1) = a0 + a1 + a2 + a3) /\
                (a1 > (&0)) /\
                (a2 > (&0)) /\
                (a3 > (&0))))`;;

(* there are some geometry theorems that should be proved here to make
        sure that everything is as expected.  The edge constraints on
        a quad cluster constrain the polygon under v1 v2 v3 v4 on the
        unit sphere to be a simple polygon (fact).
        The edge constraints that the diagonals are at least sqrt8
        together with the constraint that the quad region has area
        at most 2Pi, constrains the region to be convex (fact).  Thus, we
        can get by without proving the Jordan curve theorem for now.
        An enclosed point will be one the lies over one of the two
        simplices formed by drawing either diagonal. *)

        (* a quad cluster with no flat quarters, and diagonals at least
                sqrt8 *)
let is_sqrt_quadable = kepler_def(`is_sqrt2_quadable v0 v1 v2 v3 v4 = 
        (is_quad_cluster_v v0 v1 v2 v3 v4) /\
        (~(is_pairflat13 v0 v1 v2 v3 v4)) /\
        (~(is_pairflat24 v0 v1 v2 v3 v4))`);;


        (* define this the same way. The only difference will be in
        the scoring approximation.  These are the ones that have an
        upright diagonal of ht <= sqrt8, and for which the formulation
        bounds of vor0 and -1.04 pt apply *)
let is_mixed_quadable = kepler_def(`is_mixed_quadable v0 v1 v2 v3 v4 =
        (is_quad_cluster_v v0 v1 v2 v3 v4) /\
        (~(is_pairflat13 v0 v1 v2 v3 v4))/\
        (~(is_pairflat24 v0 v1 v2 v3 v4))`);;

        (* define an approximation to sigma.  It will be
                actual score if two flat quarters
                highest score (w) if four upright quarters in an oct
                vor(.,sqrt2) if sqrt_quadable
                max(-1.04pt,vor0) if mixed_quadable. *)

let eta_v = kepler_def(`
        eta_v v (i:num) j k = 
              let v1 = (v i) in
              let v2 = (v j) in
              let v3 = (v k) in
              let y1 = d_euclid v2 v3 in
              let y2 = d_euclid v3 v1 in
              let y3 = d_euclid v1 v2 in
              eta_y y1 y2 y3`);;

let edge_of_v = kepler_def(`edge_of_v v0 v1 v2 v3 =
        (d_euclid v0  v1,d_euclid v0  v2,d_euclid v0  v3,
        d_euclid v2 v3,d_euclid v1 v3,d_euclid v1 v2)`);;

let mu_flat_v = kepler_def(`mu_flat_v v0 v1 v2 v3 =
        let (x1,x2,x3,x4,x5,x6) = edge_of_v v0 v1 v2 v3 in
        mu_flat_x x1 x2 x3 x4 x5 x6`);;

let mu_upright_v = kepler_def(`mu_upright_v v0 v1 v2 v3 =
        let (x1,x2,x3,x4,x5,x6) = edge_of_v v0 v1 v2 v3 in
        mu_upright_x x1 x2 x3 x4 x5 x6`);;

let sigma_upright_c21_x = kepler_def(`sigma_upright_c21_x 
        x1 x2 x3 x4 x5 x6=
        mu_upright_x x1 x2 x3 x4 x5 x6`);;

let sigma_upright_c40_x = kepler_def(`sigma_upright_c40_x 
        x1 x2 x3 x4 x5 x6=
        ((&1)/(&2))*
        ((mu_upright_x x1 x2 x3 x4 x5 x6) +
         (mu_upright_x x1 x5 x6 x4 x2 x3))`);;

let qy_v = kepler_def(`qy_v v0 v1 v2 =
        let y1 = d_euclid v0  v1 in
        let y2 = d_euclid v0  v2 in
        let y3 = d_euclid v1 v2 in
        qy y1 y2 y3`);;

let full_phit = kepler_def(`full_phit h t =
        ((&1)- (h/t))*((phi h t)-(phi t t))`);;

let vort_quad_v = kepler_def(`vort_quad_v v0 v1 v2 v3 v4 t=
        let sol = solid_area_quad_v v0 v1 v2 v3 v4 in
        let phit= phi t t in
        let qy12 = (qy_v v0 v1 v2 t) + (qy_v v0 v2 v1 t) in
        let qy23 = (qy_v v0 v2 v3 t) + (qy_v v0 v3 v2 t) in
        let qy34 = (qy_v v0 v3 v4 t) + (qy_v v0 v4 v3 t) in
        let qy41 = (qy_v v0 v4 v1 t) + (qy_v v0 v1 v4 t) in
        let d1 = or_dih_v v0 v1 v2 v4 in
        let d2 = or_dih_v v0 v2 v3 v1 in
        let d3 = or_dih_v v0 v3 v4 v2 in
        let d4 = or_dih_v v0 v4 v1 v3 in
        sol*phit - (&4)*doct*(qy12+qy23+qy34+qy41) +
                (d1*(full_phit ((d_euclid v0  v1)/(&2)) t)) +
                (d2*(full_phit ((d_euclid v0  v2)/(&2)) t)) +
                (d3*(full_phit ((d_euclid v0  v3)/(&2)) t)) +
                (d4*(full_phit ((d_euclid v0  v4)/(&2)) t))`);;



(* score of an octahedron with an upright diagonal w *)
let sigma_upoct_approx_w = kepler_def(`sigma_upoct_approx_w v0 w v1 v2 v3 v4=
        let t1 = 
           (let (x1,x2,x3,x4,x5,x6) = edge_of_v v0 w v1 v2 in
           sigma_upright_c40_x x1 x2 x3 x4 x5 x6) in
        let t2 = 
           (let (x1',x2',x3',x4',x5',x6') = edge_of_v v0 w v2 v3 in
           sigma_upright_c40_x x1' x2' x3' x4' x5' x6') in
        let t3 = 
           (let (x1'',x2'',x3'',x4'',x5'',x6'') = edge_of_v v0 w v3 v4 in
           sigma_upright_c40_x x1'' x2'' x3'' x4'' x5'' x6'') in
        let t4 = 
           (let (x1''',x2''',x3''',x4''',x5''',x6''') = edge_of_v v0 w v4 v1 in
           sigma_upright_c40_x x1''' x2''' x3''' x4''' x5''' x6''') in
        t1+t2+t3+t4`);;

(* this is an upper bound on the score of a quad cluster *)
let sigma_quad_approx1 = kepler_def(`sigma_quad_approx1 v0 v1 v2 v3 v4=
        let nonoctpart = (
        if ((is_pairflat13 v0 v1 v2 v3 v4)/\
                (is_pairflat24 v0 v1 v2 v3 v4)) then
                (max_real ((mu_flat_v v0 v2 v1 v3)+(mu_flat_v v0 v4 v1 v3))
                        ((mu_flat_v v0 v1 v2 v4)+(mu_flat_v v0 v3 v2 v4)))
        else if (is_pairflat13 v0 v1 v2 v3 v4)
                then ((mu_flat_v v0 v2 v1 v3)+(mu_flat_v v0 v4 v1 v3))
        else if (is_pairflat24 v0 v1 v2 v3 v4)
                then (((mu_flat_v v0 v1 v2 v4)+(mu_flat_v v0 v3 v2 v4)))
        else 
                (max_real
                        (min_real (--(&104)*pt/(&100)) 
                                (vort_quad_v v0 v1 v2 v3 v4 t0)) 
                        (vort_quad_v v0 v1 v2 v3 v4 sqrt2))) in
        let octpart = (if (?w. (upright_oct_v v0 w v1 v2 v3 v4)) then 
                (sup (\x. ?w. ((upright_oct_v v0 w v1 v2 v3 v4) /\
                        (x = sigma_upoct_approx_w v0 w v1 v2 v3 v4))))
        else nonoctpart) in
                (max_real octpart nonoctpart)`);;

let sigma_quad_approx1_lambda = kepler_def(`sigma_quad_approx1_lambda
        v0 v1 v2 v3 v4 lambda =
                (sigma_quad_approx1 v0 v1 v2 v3 v4) -
                (solid_area_quad_v v0 v1 v2 v3 v4)*lambda*zeta*pt`);;









  