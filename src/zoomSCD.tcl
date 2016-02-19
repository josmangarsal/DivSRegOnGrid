#!/usr/bin/wish

proc Mayor { x y } {
 if { $x < $y } {
    return $y
 }
 return $x
}
proc Menor { x y } {
 if { $x < $y } {
    return $x
 }
 return $y
}

proc BoxInit {c} {
  bind $c <Button-1> {BoxBegin %W %x %y}
  bind $c <B1-Motion> {BoxDrag %W %x %y}
  bind $c <B1-ButtonRelease> {BoxEnd %W %x %y}
  bind $c <Button-3> {BoxUnZoom %W}
  bind $c <Button-2> {LowerElement %W %x %y}
}

proc RaiseElement { w x y} {
  set item [$w find closest $x $y]
  $w raise $item Trian

}

proc LowerElement { w x y} {
  set item [$w find closest $x $y]
  $w lower $item Trian

}

proc BoxBegin { w x y } {
  global box
  set box(anchor) [list $x $y]
  catch {unset box(last)}
}

proc BoxDrag { w x y } {
  global box
  catch {$w delete $box(last)}
  set x0 [lindex $box(anchor) 0]
  set y0 [lindex $box(anchor) 1]
  set dist [Mayor [expr abs($x0-$x)] [expr abs($y0-$y)]]
  set x1 [expr $x0 - $dist]
  set x2 [expr $x0 + $dist]
  set y1 [expr $y0 - $dist]
  set y2 [expr $y0 + $dist]
  set box(last) [eval {$w create rect} {$x1 $y1 $x2 $y2 -tag box -outline red}]
}

proc BoxEnd { w x y } {
  global box Z ZLevel

  catch {$w delete $box(last)}

  set dist [Mayor [expr abs ([lindex $box(anchor) 0]-$x)] \
                  [expr abs ([lindex $box(anchor) 1]-$y)]]

  if {$dist > 2} {

     set ZLevel [expr $ZLevel +1]

     set Z(xmed$ZLevel) [lindex $box(anchor) 0]
     set Z(ymed$ZLevel) [lindex $box(anchor) 1]
     set x1 [expr $Z(xmed$ZLevel) - $dist]
     set x2 [expr $Z(xmed$ZLevel) + $dist]
     set y1 [expr $Z(ymed$ZLevel) + $dist]
     set y2 [expr $Z(ymed$ZLevel) - $dist]

   #New ancho and new alto
   #Factor x e y es la proporcion entre la ventana inicial y la nueva.
     set Z(ancho$ZLevel)  [expr abs ($x2 - $x1)]
     set Z(alto$ZLevel)   [expr abs ($y2 - $y1)]
     set factorx [expr $Z(ancho0).0 / $Z(ancho$ZLevel).0]
     set factory [expr $Z(alto0).0 / $Z(alto$ZLevel).0]

   #desplazamiento en x y en y de los datos ya escalados
     set Z(xdesp$ZLevel) [expr $Z(xmed0) - $Z(xmed$ZLevel)]
     set Z(ydesp$ZLevel) [expr $Z(ymed0) - $Z(ymed$ZLevel)]


     $w scale all $Z(xmed$ZLevel) $Z(ymed$ZLevel) $factorx $factory
     $w move  all $Z(xdesp$ZLevel) $Z(ydesp$ZLevel)

     catch {$w delete text}
     $w create text 30 2 -text "Zoom $ZLevel" -fill Red -tag text

     update idletask
  }
}

proc BoxUnZoom {w} {
  global box Z ZLevel

  catch {$w delete text}

  if { $ZLevel > 0 } {
     set factorx [expr $Z(ancho$ZLevel).0 / $Z(ancho0).0]
     set factory [expr $Z(alto$ZLevel).0 / $Z(alto0).0]

     $w scale all $Z(xmed0) $Z(ymed0) $factorx $factory
     $w move  all [expr -1.0 * $Z(xdesp$ZLevel)]  [expr -1.0 * $Z(ydesp$ZLevel)]

     set ZLevel [expr $ZLevel -1]

     if { $ZLevel > 0 } {
        $w create text 30 2 -text "Zoom $ZLevel" -fill Red -tag text
     }
  }
}

proc ObjectZoom { w objeto } {
  global box Z ZLevel

  if { $ZLevel > 0 } {
     for { set i 1 } {$i <= $ZLevel } {incr i} {
	 set factorx [expr $Z(ancho0).0 / $Z(ancho$i).0]
	 set factory [expr $Z(alto0).0 / $Z(alto$i).0]
	 $w scale $objeto $Z(xmed$i) $Z(ymed$i) $factorx $factory
	 $w move  $objeto $Z(xdesp$i) $Z(ydesp$i)
     }
  }

}


##############         PINCIPAL       #################
set tcl_precision 2

set ZLevel 0


#Alto y ancho de la ventana de vision.
gets stdin Z(ancho$ZLevel)
gets stdin Z(alto$ZLevel)

set Z(xmed$ZLevel) [expr $Z(ancho0) / 2.0]
set Z(ymed$ZLevel) [expr $Z(alto0) / 2.0]

#xmin... son los valores de la ventana real
gets stdin xmin
gets stdin xmax
gets stdin ymin
gets stdin ymax
gets stdin epsi
gets stdin funcion


set font -*-fixed-*-*-*-*-*-[expr $Z(ancho0) / 4 + [expr $Z(ancho0) / 4 % 2] ]-*-*-*-*-*-*
set font2 -*-symbol-*-*-*-*-*-[expr $Z(ancho0) / 2 + [expr $Z(ancho0) / 2 % 2]]-*-*-*-*-*-*
toplevel .win
wm title .win $funcion
wm iconname .win $funcion

#set NTrian 0

frame .win.fup -bg White
label .win.fup.funcion -text $funcion -font $font -bg White
#label .win.fup.nsimplex -textvariable NTrian -font $font -bg White
pack  .win.fup.funcion -side left
#pack  .win.fup.nsimplex -side right


frame .win.fbottom -bg White
label .win.fbottom.tepsi -text "\u3F2 =" -font $font -bg White
label .win.fbottom.epsi -textvariable epsi -font $font -bg White
pack  .win.fbottom.epsi -side right
pack  .win.fbottom.tepsi -side right

canvas .win.c -width $Z(ancho0) -heigh $Z(alto0) -bg Gray80 -borderwidth 0 \
          -highlightthickness 0
#.win.c create rect 0 0 [expr $Z(ancho0)-1] [expr $Z(alto0)-1] -outline Black


pack .win.fup -side top -fill x
pack .win.c -side top
pack .win.fbottom -side top -fill x

set sigue 0
frame .win.f -borderwidth 2 -relief sunken
button .win.f.b -command exit -text "EXIT" -padx 2 -pady 2
button .win.f.c -text "Postscript" -padx 2 -pady 2 -command\
	{.win.c postscript -file Blending.ps}
button .win.f.f -text "CONTINUE" -padx 2 -pady 2 -command {incr sigue}
pack .win.f -side top -fill x
pack .win.f.b -side left
pack .win.f.c -side left
pack .win.f.f -side right


BoxInit {.win.c}
update idletask

# GRID
#set centroalto [expr {$Z(alto0) / 2}]
#set centroancho [expr {$Z(ancho0) / 2}]
#set Point [.win.c find withtag PointPrueba]
#.win.c create oval $centroancho $centroalto $centroancho $centroalto -outline blue -width 12 -tag CirclePrueba
#.win.c create oval [expr $centroancho / 2] [expr $centroalto / 2] [expr $centroancho / 2] [expr $centroalto / 2] -outline blue -width 12 -tag CirclePrueba
#.win.c create oval [expr $centroancho + $centroancho / 2] [expr $centroalto / 2] [expr $centroancho + $centroancho / 2] [expr $centroalto / 2] -outline blue -width 12 -tag CirclePrueba
#.win.c create oval [expr $centroancho / 2] [expr $centroalto + $centroalto / 2] [expr $centroancho / 2] [expr $centroalto + $centroalto / 2] -outline blue -width 12 -tag CirclePrueba
#.win.c create oval [expr $centroancho + $centroancho / 2] [expr $centroalto + $centroalto / 2] [expr $centroancho + $centroancho / 2] [expr $centroalto + $centroalto / 2] -outline blue -width 12 -tag CirclePrueba
##ObjectZoom .win.c CirclePrueba
##.win.c addtag Point withtag CirclePrueba

while { 0 < [gets stdin accion] } {
  switch $accion {
        "DrawTriangle" {
		    gets stdin x1
		    gets stdin y1
		    gets stdin x2
		    gets stdin y2
		    gets stdin x3
		    gets stdin y3
		    gets stdin colorIn
		    gets stdin NTrian
		    catch { .win.c delete Trian$NTrian }
		    .win.c create polygon $x1 $y1 $x2 $y2 $x3 $y3 $x1 $y1\
		     -fill $colorIn -outline Black -width 1 -tag Trian$NTrian
#		    .win.c create line $x1 $y1 $x2 $y2 $x3 $y3 $x1 $y1 \
#		     -fill $colorIn  -width 2 -tag Trian$NTrian

		    ObjectZoom .win.c Trian$NTrian

		    .win.c addtag Trian withtag Trian$NTrian

		    catch {.win.c raise Point Trian$NTrian}
		    catch {.win.c raise PC Trian$NTrian}
		    catch {.win.c raise Const Trian$NTrian}
		    catch {.win.c raise EpsCircle Trian$NTrian}
		    catch {.win.c raise LineTrian Trian$NTrian}
		    catch {.win.c raise LineCoorTrian Trian$NTrian}
#		    tkwait variable sigue
                    update idletask
	}
        "DelTriangle" {
                   gets stdin NTrian
                   catch { .win.c delete Trian$NTrian }
#		   tkwait variable sigue
                   update idletask
	}
        "DrawLineTriangle" {
		    gets stdin x1
		    gets stdin y1
		    gets stdin x2
		    gets stdin y2
		    gets stdin x3
		    gets stdin y3
		    gets stdin Color
		    gets stdin NTrian
#		    if { $NTrian == 0 } {
		    catch { .win.c delete LineTrian$NTrian }
		    .win.c create line $x1 $y1 $x2 $y2 $x3 $y3 $x1 $y1\
		     -fill $Color -width 3 -tag LineTrian$NTrian
#For grid, comment Del triangle		    }
#Added for SCD: draw also the vertices
		    catch { .win.c delete LineTrianV1$NTrian }

		    .win.c create rect [expr $x1-4] [expr $y1-4] \
		    [expr $x1+4] [expr $y1+4]  -fill red \
		    -width 1 -fill red -outline black \
		     -tag LineTrianV1$NTrian
		     
		    .win.c create rect [expr $x2-4] [expr $y2-4] \
		    [expr $x2+4] [expr $y2+4]  -fill red\
		    -width 1 -fill red -outline black \
		     -tag LineTrianV2$NTrian

		    .win.c create rect [expr $x3-4] [expr $y3-4] \
		    [expr $x3+4] [expr $y3+4]  -fill red\
		    -width 1 -fill red -outline black \
		     -tag LineTrianV3$NTrian

		    ObjectZoom .win.c LineTrian$NTrian
		    ObjectZoom .win.c LineTrianV1$NTrian
		    ObjectZoom .win.c LineTrianV2$NTrian
		    ObjectZoom .win.c LineTrianV3$NTrian


		    .win.c addtag LineTrian withtag LineTrian$NTrian
		    .win.c addtag LineTrianV1 withtag LineTrianV1$NTrian
		    .win.c addtag LineTrianV2 withtag LineTrianV2$NTrian
		    .win.c addtag LineTrianV3 withtag LineTrianV3$NTrian

		    catch {.win.c raise Point LineTrian$NTrian}
		    catch {.win.c raise PC LineTrian$NTrian}
		    catch {.win.c raise Const LineTrian$NTrian}
		    catch {.win.c raise EpsCircle LineTrian$NTrian}
		    catch {.win.c raise LineTrianV1 LineTrian$NTrian}
		    catch {.win.c raise LineTrianV2 LineTrian$NTrian}
		    catch {.win.c raise LineTrianV3 LineTrian$NTrian}
		    catch {.win.c raise LineTrianV1 Point$NPoint}
		    catch {.win.c raise LineTrianV2 Point$NPoint}
		    catch {.win.c raise LineTrianV3 Point$NPoint}
#		    tkwait variable sigue
                    update idletask
	}
        "DelLineTriangle" {
                   gets stdin NTrian
                   catch { .win.c delete LineTrian$NTrian }
                   catch { .win.c delete LineTrianV1$NTrian }
                   catch { .win.c delete LineTrianV2$NTrian }
                   catch { .win.c delete LineTrianV3$NTrian }
#		   tkwait variable sigue
                   update idletask
	}
        "DrawLineCoorTriangle" {
		    gets stdin x1
		    gets stdin y1
		    gets stdin x2
		    gets stdin y2
		    gets stdin x3
		    gets stdin y3
		    gets stdin Color
		    gets stdin NTrian
		    catch { .win.c delete LineCoorTrian$NTrian }
		    .win.c create line $x1 $y1 $x2 $y2 $x3 $y3 $x1 $y1\
		     -fill $Color -width 3 -tag LineCoorTrian$NTrian

		    ObjectZoom .win.c LineCoorTrian$NTrian

		    .win.c addtag LineCoorTrian withtag LineCoorTrian$NTrian

		    catch {.win.c raise Point LineCoorTrian$NTrian}
		    catch {.win.c raise PC LineCoorTrian$NTrian}
		    catch {.win.c raise Const LineCoorTrian$NTrian}
		    catch {.win.c raise EpsCircle LineCoorTrian$NTrian}
#		    tkwait variable sigue
                    update idletask
	}
        "DelLineCoorTriangle" {
                   gets stdin NTrian
                   catch { .win.c delete LineCoorTrian$NTrian }
#		   tkwait variable sigue
                   update idletask
	}
        "DrawLine" {
		    gets stdin x1
		    gets stdin y1
		    gets stdin x2
		    gets stdin y2
		    gets stdin colorIn
		    gets stdin NTrian
		    .win.c create line $x1 $y1 $x2 $y2 \
		     -fill $colorIn -width 2 -tag Trian$NTrian

		    ObjectZoom .win.c Trian$NTrian

		    catch {.win.c raise Point Trian$NTrian}
		    catch {.win.c raise Const Trian$NTrian}
#		    tkwait variable sigue
                    update idletask
	}
        "DelLine" {
                   gets stdin NTrian
                   catch { .win.c delete Trian$NTrian }
#		   tkwait variable sigue
                   update idletask
	}
        "DrawPoint" {
		    gets stdin x1
		    gets stdin y1
		    gets stdin color
		    gets stdin NPoint
		    set border black

		    set Point [.win.c find withtag Point$NPoint]
		    if { [llength $Point] == 0 } {
		    .win.c create rect [expr $x1-10] [expr $y1-10] \
		    [expr $x1+10] [expr $y1+10]  -fill $color\
		    -width 1 -fill $color -outline $border \
		     -tag Point$NPoint

                    ObjectZoom .win.c Point$NPoint

		    .win.c addtag Point withtag Point$NPoint

#		    tkwait variable sigue
                    }
                    unset Point
                    update idletask
	}
        "DelPoint" {
                   gets stdin NPoint
                   catch { .win.c delete Point$NPoint }
#		   tkwait variable sigue
                   update idletask
	}
	"DrawConst" {
		   gets stdin cuantos
		   gets stdin color
		   gets stdin ldash
		   gets stdin NConst
		   set Puntos {}
		   for {set j 0} { $j < $cuantos} {incr j} {
        		gets stdin x1
        		gets stdin x2
        		lappend Puntos $x1 $x2
		    }

		    eval {.win.c create line} $Puntos \
		    -tag Const$NConst -width 3 -fill $color \
		    -smooth off -dash  $ldash

		    .win.c addtag Const withtag Const$NConst
		    catch {.win.c raise Point Const$NConst}
#		    tkwait variable sigue

                    update idletask
	}
	"DrawCircle" {
		    gets stdin x1
		    gets stdin y1
		    gets stdin x2
		    gets stdin y2
		    gets stdin color
		    gets stdin ldash
		    gets stdin NCircle

		    .win.c create oval $x1 $y1 $x2 $y2 \
		     -outline $color -width 3 -dash $ldash \
		     -tag  Circle$NCircle

		    ObjectZoom .win.c Circle$NCircle

		    .win.c addtag Circle withtag Circle$NCircle
		    catch {.win.c raise Point Circle$NCircle}
		    catch {.win.c raise PC Circle$NCircle}
#		    tkwait variable sigue

                    update idletask
	}
        "DelCircle" {
                   gets stdin NCircle
                   catch { .win.c delete Circle$NCircle }
#		   .win.c scale all 0 0 1.1 1.1
#		   tkwait variable sigue
                   update idletask
	}
	"DrawEpsCircle" {
		    gets stdin x1
		    gets stdin y1
		    gets stdin x2
		    gets stdin y2
		    gets stdin OutLineColor
		    gets stdin FillColor
		    gets stdin NCircle

		    .win.c create oval $x1 $y1 $x2 $y2 \
		      -outline $OutLineColor -width 3 \
		      -tag EpsCircle$NCircle

		    ObjectZoom .win.c EpsCircle$NCircle

		    .win.c addtag EpsCircle withtag EpsCircle$NCircle
		    catch {.win.c raise Point EpsCircle$NCircle}
		    catch {.win.c raise PC EpsCircle$NCircle}
#		    tkwait variable sigue

                    update idletask
	}
        "DelEpsCircle" {
                   gets stdin NCircle
                   catch { .win.c delete EpsCircle$NCircle }
#		   tkwait variable sigue
                   update idletask
	}
        "DrawPCover" {
		    gets stdin x1
		    gets stdin y1
		    gets stdin color
		    gets stdin NPC
		    set border black

		    .win.c create rect [expr $x1-0.1] [expr $y1-0.1] \
		    [expr $x1+0.1] [expr $y1+0.1] \
		     -width 3 -fill $color -outline $color -tag PC$NPC

		    ObjectZoom .win.c PC$NPC

		    .win.c addtag PC withtag PC$NPC
#		    tkwait variable sigue

                    update idletask
	}
        "DelPCover" {
                   gets stdin NPC
                   catch { .win.c delete PC$NPC }
#		   tkwait variable sigue
                   update idletask
	}

        "Wait" {
	           tkwait variable sigue
                   update idletask
	}
  }
}
update idletask
