########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
#  following conditions are met:
#
#  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
#  disclaimer.
#
#  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
#  disclaimer in the documentation and/or other materials provided with the distribution.
#
#  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
#  derived from this software without specific prior written permission from the respective party.
#
#  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
#  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
#  written permission from Alliance for Sustainable Energy, LLC.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
#  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
#  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
#  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
#  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
########################################################################################################################
require 'openstudio'
require 'logger'
require 'openstudio/measure/ShowRunnerOutput'
require 'fileutils'
require 'minitest/autorun'
require 'csv'

class RemoveSpikesAndOverlaps_TZ46_TZ47_Test < Minitest::Test
  def test_RemoveSpikesAndOverlaps_TZ46_TZ47

    model = OpenStudio::Model::Model.new
    space = OpenStudio::Model::Space.new(model)
    assert(space.name)
    puts space.name
    assert_equal("Space 1", space.name.get); #TZ46-81

    #module AMod # you can put the module into a file, too
    #points = Array.new
    points = OpenStudio::Point3dVector.new
    points.push(OpenStudio::Point3d.new(0.787401574803132, 3.14960629921254, 3.84078906495842e-15));
    points.push(OpenStudio::Point3d.new(2.75590551181098, 1.18110236220469, 7.70979364755774e-15));
    points.push(OpenStudio::Point3d.new(2.00599925009369, 0.356205474315723, 3.76899861514623e-15));
    points.push(OpenStudio::Point3d.new(0, 2.36220472440939, 0));

    surface1 = OpenStudio::Model::Surface.new(points,model)
    surface1.setParent(space)

    points.clear

    points.push(OpenStudio::Point3d.new(1.37795275590549, 0.984251968503903, 3.51739188744401));
    points.push(OpenStudio::Point3d.new(5.31496062992121, 4.92125984251968, 3.51739188744401));
    points.push(OpenStudio::Point3d.new(3.93700787401569, 6.2992125984252, 3.51739188744401));
    points.push(OpenStudio::Point3d.new(0, 2.36220472440939, 3.51739188744401));

    surface2 = OpenStudio::Model::Surface.new(points,model)
    surface2.setParent(space)

    points.clear
    points.push(OpenStudio::Point3d.new(5.11811023622049, 2.75590551181099, 3.51739188744401));
    points.push(OpenStudio::Point3d.new(5.11811023622049, 2.75590551181099, 0));
    points.push(OpenStudio::Point3d.new(6.2992125984252, 3.93700787401571, 0));
    points.push(OpenStudio::Point3d.new(6.2992125984252, 3.93700787401571, 3.51739188744401));

    surface3 = OpenStudio::Model::Surface.new(points,model)
    surface3.setParent(space)

    points.clear
    points.push(OpenStudio::Point3d.new(2.36220472440939, 0, 3.51739188744401));
    points.push(OpenStudio::Point3d.new(2.36220472440939, 0, 0));
    points.push(OpenStudio::Point3d.new(5.11811023622049, 2.75590551181099, 0));
    points.push(OpenStudio::Point3d.new(5.11811023622049, 2.75590551181099, 3.51739188744401));

    surface4 = OpenStudio::Model::Surface.new(points,model)
    surface4.setParent(space)

    points.clear
    points.push(OpenStudio::Point3d.new(0, 2.36220472440939, 3.51739188744401));
    points.push(OpenStudio::Point3d.new(0, 2.36220472440939, 0));
    points.push(OpenStudio::Point3d.new(2.36220472440939, 0, 0));
    points.push(OpenStudio::Point3d.new(2.36220472440939, 0, 3.51739188744401));

    surface5 = OpenStudio::Model::Surface.new(points,model)
    surface5.setParent(space)

    points.clear
    points.push(OpenStudio::Point3d.new(3.93700787401569, 6.2992125984252, 3.51739188744401));
    points.push(OpenStudio::Point3d.new(3.93700787401569, 6.2992125984252, 0));
    points.push(OpenStudio::Point3d.new(0, 2.36220472440939, 0));
    points.push(OpenStudio::Point3d.new(0, 2.36220472440939, 3.51739188744401));

    surface6 = OpenStudio::Model::Surface.new(points,model)
    surface6.setParent(space)

    points.clear
    points.push(OpenStudio::Point3d.new(6.2992125984252, 3.93700787401571, 3.51739188744401));
    points.push(OpenStudio::Point3d.new(6.2992125984252, 3.93700787401571, 0));
    points.push(OpenStudio::Point3d.new(3.93700787401569, 6.2992125984252, 0));
    points.push(OpenStudio::Point3d.new(3.93700787401569, 6.2992125984252, 3.51739188744401));

    surface7 = OpenStudio::Model::Surface.new(points,model)
    surface7.setParent(space)

    points.clear
    points.push(OpenStudio::Point3d.new(2.3622047244094, -1.44382283906452e-15, 3.51739188744401));
    points.push(OpenStudio::Point3d.new(6.2992125984252, 3.93700787401571, 3.51739188744401));
    points.push(OpenStudio::Point3d.new(5.31496062992121, 4.92125984251968, 3.51739188744401));
    points.push(OpenStudio::Point3d.new(1.37795275590549, 0.984251968503901, 3.51739188744401));

    surface8= OpenStudio::Model::Surface.new(points,model)
    surface8.setParent(space)

    points.clear
    points.push(OpenStudio::Point3d.new(3.93700787401569, 6.2992125984252, 0));
    points.push(OpenStudio::Point3d.new(6.2992125984252, 3.93700787401571, 0));
    points.push(OpenStudio::Point3d.new(2.3622047244094, -2.88764567812905e-15, 0));
    points.push(OpenStudio::Point3d.new(2.00599925009369, 0.356205474315719, 4.33146851719357e-15));
    points.push(OpenStudio::Point3d.new(2.75590551181099, 1.18110236220469, 7.21911419532262e-15));
    points.push(OpenStudio::Point3d.new(0.787401574803138, 3.14960629921254, 4.33146851719357e-15));

    surface9 = OpenStudio::Model::Surface.new(points,model)
    surface9.setParent(space)

    space2 = OpenStudio::Model::Space.new(model)
    assert(space2.name)
    puts space2.name
    assert_equal("Space 2", space2.name.get)  #TZ47-91

    points.clear
    points.push(OpenStudio::Point3d.new(7.0866141732284, 9.44881889763781, 0))
    points.push(OpenStudio::Point3d.new(8.16034359341449, 8.55404438081602, -1.44382283906452e-15))
    points.push(OpenStudio::Point3d.new(5.90551181102359, 6.2992125984252, -1.44382283906452e-15))
    points.push(OpenStudio::Point3d.new(4.92125984251972, 7.28346456692913, -7.21911419532262e-16))

    surface10 = OpenStudio::Model::Surface.new(points,model)
    surface10.setParent(space2)

    points.clear
    points.push(OpenStudio::Point3d.new(1.96850393700789, -2.88764567812905e-15, 6.095999804928))
    points.push(OpenStudio::Point3d.new(9.44881889763779, 7.4803149606299, 6.095999804928))
    points.push(OpenStudio::Point3d.new(7.0866141732284, 9.44881889763781, 6.095999804928))
    points.push(OpenStudio::Point3d.new(-5.77529135625809e-15, 2.3622047244094, 6.095999804928))

    surface11 = OpenStudio::Model::Surface.new(points,model)
    surface11.setParent(space2)

    points.clear
    points.push(OpenStudio::Point3d.new(7.0866141732284, 9.44881889763781, 6.095999804928))
    points.push(OpenStudio::Point3d.new(7.0866141732284, 9.44881889763781, 0))
    points.push(OpenStudio::Point3d.new(-5.77529135625809e-15, 2.3622047244094, 0))
    points.push(OpenStudio::Point3d.new(-5.77529135625809e-15, 2.3622047244094, 6.095999804928))

    surface12 = OpenStudio::Model::Surface.new(points,model)
    surface12.setParent(space2)

    points.clear
    points.push(OpenStudio::Point3d.new(9.44881889763779, 7.4803149606299, 6.095999804928))
    points.push(OpenStudio::Point3d.new(9.44881889763779, 7.4803149606299, 0))
    points.push(OpenStudio::Point3d.new(7.0866141732284, 9.44881889763781, 0))
    points.push(OpenStudio::Point3d.new(7.0866141732284, 9.44881889763781, 6.095999804928))

    surface13 = OpenStudio::Model::Surface.new(points,model)
    surface13.setParent(space2)

    points.clear
    points.push(OpenStudio::Point3d.new(1.96850393700789, -2.88764567812905e-15, 6.095999804928))
    points.push(OpenStudio::Point3d.new(1.96850393700789, -2.88764567812905e-15, 0))
    points.push(OpenStudio::Point3d.new(9.44881889763779, 7.4803149606299, 0))
    points.push(OpenStudio::Point3d.new(9.44881889763779, 7.4803149606299, 6.095999804928))

    surface14 = OpenStudio::Model::Surface.new(points,model)
    surface14.setParent(space2)

    points.clear
    points.push(OpenStudio::Point3d.new(-5.77529135625809e-15, 2.3622047244094, 6.095999804928))
    points.push(OpenStudio::Point3d.new(-5.77529135625809e-15, 2.3622047244094, 0))
    points.push(OpenStudio::Point3d.new(1.96850393700789, -2.88764567812905e-15, 0))
    points.push(OpenStudio::Point3d.new(1.96850393700789, -2.88764567812905e-15, 6.095999804928))

    surface15 = OpenStudio::Model::Surface.new(points,model)
    surface15.setParent(space2)

    points.clear
    points.push(OpenStudio::Point3d.new(8.16034359341449, 8.55404438081602, -1.44382283906452e-15))
    points.push(OpenStudio::Point3d.new(9.44881889763779, 7.4803149606299, 0))
    points.push(OpenStudio::Point3d.new(1.96850393700789, -2.88764567812905e-15, 0))
    points.push(OpenStudio::Point3d.new(-5.77529135625809e-15, 2.3622047244094, 0))
    points.push(OpenStudio::Point3d.new(4.92125984251972, 7.28346456692913, -7.21911419532262e-16))
    points.push(OpenStudio::Point3d.new(5.90551181102359, 6.2992125984252, -1.44382283906452e-15))

    surface16 = OpenStudio::Model::Surface.new(points,model)
    surface16.setParent(space2)

    assert_equal(2, model.getSpaces.size)
    assert_equal(16, model.getSurfaces.size)
    assert(true)
    _vector1 = OpenStudio::Vector3d.new(-34.6329065993854, 2.80342559613037, -9.61339169237201)
    _vector2 = OpenStudio::Vector3d.new(0,0,1)
    _radVal_relativeToNorth = -OpenStudio::degToRad(24.5)
    _rotation = OpenStudio::Transformation::rotation(_vector2,_radVal_relativeToNorth)
    #_transformation = OpenStudio::Transformation::translation()

    space.setXOrigin(-34.6329065993854)
    space.setYOrigin(2.80342559613037)
    space.setZOrigin(-9.61339169237201)
    space.setDirectionofRelativeNorth(24.5)
    assert(space.transformation)
    assert_equal(-34.6329065993854, space.xOrigin)
    assert_equal(2.80342559613037, space.yOrigin)
    assert_equal(-9.61339169237201, space.zOrigin)
    assert_equal(24.5, space.directionofRelativeNorth)
    #assert(space.setTransformation(OpenStudio::Transformation::translation(OpenStudio::Vector3d.new(-34.6329065993854, 2.80342559613037, -9.61339169237201))* OpenStudio::Transformation::rotation(OpenStudio::Vector3d.new(0, 0, 1), -OpenStudio::degToRad(24.5))))

    space2.setXOrigin(-40.4694197176487)
    space2.setYOrigin(-1.45922839997927)
    space2.setZOrigin(-6.095999804928)
    space2.setDirectionofRelativeNorth(24.5)
    assert(space2.transformation)
    assert_equal(-40.4694197176487, space2.xOrigin)
    assert_equal(-1.45922839997927, space2.yOrigin)
    assert_equal(-6.095999804928, space2.zOrigin)
    assert_equal(24.5, space2.directionofRelativeNorth)
    # assert(space2.setTransformation(OpenStudio::Transformation::translation(OpenStudio::Vector3d.new(-40.4694197176487, -1.45922839997927, -6.095999804928)) * OpenStudio::Transformation::rotation(OpenStudio::Vector3d.new(0, 0, 1), -OpenStudio::degToRad(24.5))));

    spaces = model.getSpaces;
    blacklist = OpenStudio::Model::SpaceVector.new

    nSurfaces = 0
    spaces.each do |space|
      nSurfaces += space.surfaces.size
    end

    puts "The Model has total #{spaces.size} spaces & #{nSurfaces} surfaces"
    puts "The Model has total #{spaces.size} spaces & #{nSurfaces} surfaces"

    tol = 1.0E-5
    plane_equal_count = 0
    inside_intersection_count = 0
    i_sum =0
    j_sum = 0
    model.getSpaces.each do |space|
      #spaces_filtered<<space
      name = space.name.get
      #puts "Space : #{name}"
      puts "Space : #{name}"
      spaceTransformation = space.transformation
      #puts "Trnsformation : #{spaceTransformation}"
      puts "Trnsformation : #{spaceTransformation}"

      surfaces = space.surfaces
      if name == "Space TZ46-3"
        hello = 1  #not required
        #puts "-------------------------------------------------------------------------------------------------------> Hello"
      end

      for i in 0..space.surfaces.size-1
        i_sum+=1
        thisSurface =  surfaces[i]
        thisName = thisSurface.name
        #puts "Surface Name : #{thisName}"
        puts "Surface Name : #{thisName}"
        for j in i+1..surfaces.size-1
          j_sum+=1
          otherSurface = surfaces[j]
          otherName = otherSurface.name.get
          #puts "Other Surface : #{otherSurface} & Name : #{otherName}"
          puts "Other Surface : #{otherSurface} & Name : #{otherName}"

          plane = spaceTransformation * thisSurface.plane
          #puts "Plane : #{plane}"
          puts "Plane : #{plane}"

          otherPlane = spaceTransformation * otherSurface.plane
          #puts "Plane : #{otherPlane}"
          puts "Plane : #{otherPlane}"

          if plane.equal(otherPlane)
            plane_equal_count+=1
            #puts "------------------------------------------------------------------------------------------------------------------------------------------> INSIDE PLANE AND OTHER PLANE COMPARISON TEST"
            puts "------------------------------------------------------------------------------------------------------------------------------------------> INSIDE PLANE AND OTHER PLANE COMPARISON TEST"

            thisVertices = spaceTransformation * thisSurface.vertices
            #puts "This Vertices : #{thisVertices}"
            puts "This Vertices : #{thisVertices}"

            otherVertices = spaceTransformation * otherSurface.vertices
            #puts "Other Vertices : #{otherVertices}"
            puts "Other Vertices : #{otherVertices}"

            faceTransformation = OpenStudio::Transformation::alignFace(thisVertices)
            #puts "faceTransformation : #{faceTransformation}"
            puts "faceTransformation : #{faceTransformation}"

            faceTransformationInverse = faceTransformation.inverse
            faceVertices = faceTransformationInverse * thisVertices
            otherFaceVertices = faceTransformationInverse * otherVertices

            outwardNormal = OpenStudio::getOutwardNormal(faceVertices)
            #puts "Outward Normal : #{outwardNormal.get.z}"
            puts "Outward Normal : #{outwardNormal.get.z}"

            faceVertices_R = Array.new
            if outwardNormal.get.z > -1 + tol
              faceVertices_R = faceVertices.reverse()
              # HAVE TO SAVE IT INTO DIFFERENT VARIABLE
              # TO PASS IT INTO INTERSECT
              # or Make .reverse!() working | failing as Frozen
            end

            otherOutwardNormal = OpenStudio::getOutwardNormal(otherFaceVertices)
            #puts "Other Outward Normal : #{otherOutwardNormal.get.z}"
            puts "Other Outward Normal : #{otherOutwardNormal.get.z}"

            otherFaceVertices_R = Array.new
            if otherOutwardNormal.get.z > -1 + tol
              otherFaceVertices_R = otherFaceVertices.reverse()
              # HAVE TO SAVE IT INTO DIFFERENT VARIABLE
              # TO PASS IT INTO INTERSECT
              # or Make .reverse!() working | failing as Frozen
            end

            intersection = OpenStudio::intersect(faceVertices_R, otherFaceVertices_R, tol)
            # if  intersection => all result
            # if !intersection => o result
            # if intersection.nil?
            if not intersection.empty?
              inside_intersection_count+=1
              puts "Space #{name} has surfaces that intersect with each other, namely  #{thisName} and #{otherName}"
              puts  "Space #{name} has surfaces that intersect with each other, namely  #{thisName} and #{otherName}"
              #blacklist.push(space)
              blacklist << space
            end
          elsif
            puts "------------------------------------------------------------------------------------------------------------------------------------------> OUTSIDE PLANE AND OTHER PLANE COMPARISON TEST"
            puts "------------------------------------------------------------------------------------------------------------------------------------------> OUTSIDE PLANE AND OTHER PLANE COMPARISON TEST"
          end

        end
      end

    end

    puts ""
    # puts "Plane equal count(280 as per c++) = #{plane_equal_count} in ruby"
    # puts "Insdie Intersect Count (11 as per c++) = #{inside_intersection_count} in ruby"
    # puts "I sum (986 as per c++) : #{i_sum}"
    # puts "J sum (7345 as per c++) : #{j_sum}"
    puts Time.now
    puts "All Spaces Count : #{spaces.size}"
    puts "Black List Spaces Count : #{blacklist.uniq.size}"


    spaces_filtered = spaces.dup
    puts "The initial filtered Spaces Count : #{spaces_filtered.size}"
    blacklist.each do |sp|
      #model.getSpaces.delete(sp) #Frozen array error
      #spaces.delete(sp)  # Frozen array error
      spaces_filtered.delete(sp) # working but mis-match 662 to 666
    end
    puts "The final filtered Spaces Count : #{spaces_filtered.size}"


    # put all of the spaces in the model into a vector
    spacesF = OpenStudio::Model::SpaceVector.new
    spaces_filtered.each do |space|
      spacesF << space
    end



    initialMatchedSurfaceCounter = 0
    spacesF.each do |sp|
      sp.surfaces.each do |surface|
        if surface.outsideBoundaryCondition == 'Surface'
          next if !surface.adjacentSurface.is_initialized # don't count as matched if boundary condition is right but no matched object
          initialMatchedSurfaceCounter += 1
        end
      end
    end

    # Don't sort by area, use the shrink and expand to remove spikes
    OpenStudio::Model::intersectSurfaces(spacesF)#,false,true)
    OpenStudio::Model::matchSurfaces(spacesF)

    # outdir = File.absolute_path(File.join(OpenStudio::getEnergyPlusDirectory.to_s, "../resources/model"))
    # outpath = File.join(outdir, "removeSpikesAndOverlaps_TZ46_TZ47.osm")
    # model.save(outpath, true);

  end
end
