
# vtk visualization helper functions/classes
# from: https://github.com/aewallin/openvoronoi/blob/master/python_examples/ovdvtk.py
import ovdvtk 

import time
import vtk
import datetime
import math
import random
import os

import rpg



def drawLine(myscreen, p1, p2):
    myscreen.addActor( ovdvtk.Line( p1 = (p1[0],p1[1],0), p2=(p2[0],p2[1],0), color = ovdvtk.yellow ) )

def writeFrame( w2if, lwr, n ):
    """ write a screenshot to disk """
    w2if.Modified() 
    current_dir = os.getcwd()
    filename = current_dir + "/frames/rpg_"+ ('%05d' % n)+".png"
    lwr.SetFileName( filename )
    lwr.Write()

def drawPolygon(myscreen, poly):
    prev = poly[-1] # initialize with last element
    for p in poly:
        drawLine(myscreen,prev,p)
        prev = p

def drawText(myscreen, N=1):
    t = ovdvtk.Text()
    t.SetPos( (myscreen.width-300, myscreen.height-70) )
    date_text = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    poly_text = "{0} vertices".format(N)
    t.SetText( "CGAL::random_polygon_2\n" + date_text + "\n" + poly_text)
    myscreen.addActor(t)

def drawGithub(myscreen):
    gittext  = ovdvtk.Text()
    gittext.SetPos( (50, 50) )
    gittext_text = "github.com/aewallin"
    gittext.SetText( gittext_text )
    myscreen.addActor(gittext)


if __name__ == "__main__":  
    print rpg.version()
    myscreen = ovdvtk.VTKScreen(width=1024, height=720) #(width=1920, height=1080)
    drawGithub(myscreen)
    # for screenshot writing:
    w2if = vtk.vtkWindowToImageFilter()
    w2if.SetInput(myscreen.renWin)
    lwr = vtk.vtkPNGWriter()
    lwr.SetInput( w2if.GetOutput() )
    
    myscreen.render()
    camPos = 1
    zmult = 4
    # position the camera
    myscreen.camera.SetPosition(0, -camPos/float(1000), zmult*camPos) 
    myscreen.camera.SetClippingRange(-(zmult+1)*camPos,(zmult+1)*camPos)
    myscreen.camera.SetFocalPoint(0.0, 0, 0)
    
    N=800
    drawText(myscreen,N)
    t_before = time.time() 
    polygon= rpg.rpg(N)
    t_after = time.time() 

    calctime = t_after-t_before

    print "RPG done in ", calctime," s, polygon has ", N," vertices"
    
    drawPolygon( myscreen, polygon )

    print "PYTHON All DONE."
    writeFrame( w2if, lwr, N )
    myscreen.render()    
    myscreen.iren.Start()
