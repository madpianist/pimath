import unittest

import pimath
import math

def near( arr1, arr2, tolerance ):
    if len(arr1) != len(arr2):
        return False
    for i in range( 0, len( arr1 ) ):
        if abs( arr1[i] - arr2[i] ) > tolerance:
            return False
    return True

class TestPimath(unittest.TestCase):

    def setup(self):
        pass

    def teardown(self):
        pass

    def testMatrix44(self):
        self.runMatrix44Test( pimath.M44f )
        self.runMatrix44Test( pimath.M44d )
        self.runMatrix44Test( pimath.M44h )

    def runMatrix44Test(self, cls ):
        identity = ( (1.0, 0.0, 0.0, 0.0), (0.0, 1.0, 0.0, 0.0), (0.0, 0.0, 1.0, 0.0), (0.0, 0.0, 0.0, 1.0) )

        matrix44 = cls()
        assert matrix44.value == identity

        amatrix = ( (1.0, 2.0, 3.0, 4.0), (5.0, 6.0, 7.0, 8.0), (9.0, 10.0, 11.0, 12.0), (13.0, 14.0, 15.0, 16.0) )

        matrix44 = cls( (
            (amatrix[0][0], amatrix[0][1], amatrix[0][2], amatrix[0][3]),
            (amatrix[1][0], amatrix[1][1], amatrix[1][2], amatrix[1][3]),
            (amatrix[2][0], amatrix[2][1], amatrix[2][2], amatrix[2][3]),
            (amatrix[3][0], amatrix[3][1], amatrix[3][2], amatrix[3][3]) ) )

        assert matrix44.value == amatrix

        mul = cls() * matrix44
        assert mul.value == amatrix

        matrix44 *= cls()
        assert matrix44.value == amatrix

        matrix44[2, 3] = 2.2
        #matrix44[2][3] = 2.2 would do nothing
        assert matrix44[2][3] - 2.2 < 0.001
        assert matrix44[2, 3] - 2.2 < 0.001

        matrix44.value = (
            (1.0, 0.0, 0.0, 0.0),
            (0.0, 1.0, 0.0, 0.0),
            (0.0, 0.0, 1.0, 0.0),
            (0.0, 0.0, 0.0, 1.0))

        assert matrix44.value == ((1.0, 0.0, 0.0, 0.0), (0.0, 1.0, 0.0, 0.0), (0.0, 0.0, 1.0, 0.0), (0.0, 0.0, 0.0, 1.0))

        inv = matrix44.inverse()
        matrix44.invert( True )
        assert inv.value == matrix44.value

        transp = matrix44.transposed()
        matrix44.transpose()
        assert matrix44.value == transp.value

    def testMatrix33(self):
        self.runMatrix33Test( pimath.M33f )
        self.runMatrix33Test( pimath.M33d )
        self.runMatrix33Test( pimath.M33h )

    def runMatrix33Test(self, cls):
        identity = ( (1.0, 0.0, 0.0), (0.0, 1.0, 0.0), (0.0, 0.0, 1.0) )

        matrix33 = cls()
        assert matrix33.value == identity

        amatrix = (
            (1.0, 2.0, 3.0),
            (4.0, 5.0, 6.0),
            (7.0, 8.0, 9.0))

        matrix33.value = amatrix
        assert matrix33.value == amatrix

        matrix33[0, 0] = 9.0
        # Note: matrix33[0][0] = 9.0 is NOT supported
        assert matrix33[0][0] == 9.0
        assert matrix33[0, 0] == 9.0

    def testVector(self):
        self.runVectorTest( pimath.V3i )
        self.runVectorTest( pimath.V3f )
        self.runVectorTest( pimath.V3d )
        self.runVectorTest( pimath.V3h )

    def runVectorTest(self, cls):
        vec = cls()
        assert vec.value == (0.0, 0.0, 0.0)

        if cls == pimath.V3i:
            vec.value = (1, 2, 3)
            assert vec.x - 1 < 0.00001
            assert vec.y - 2 < 0.00001
            assert vec.z - 3 < 0.00001
        elif cls == pimath.V3h:
            vec.value = (1.5, 2.2, 3.4)
            assert vec.x - 1.5 < 0.001
            assert vec.y - 2.2 < 0.001
            assert vec.z - 3.4 < 0.001
        else:
            vec.value = (1.5, 2.2, 3.4)
            assert vec.x - 1.5 < 0.00001
            assert vec.y - 2.2 < 0.00001
            assert vec.z - 3.4 < 0.00001

        assert cls(0, 1, 0).dot(cls(1, 0, 0)) == 0.0

        crossVec = cls(0, 1, 0).cross(cls(1, 0, 0))
        assert crossVec.value == (0.0, 0.0, -1.0)

        assert (cls(0, 2, 0) * cls(0, 3, 0)).value == (0.0, 6.0, 0.0)

        vec = cls(0, 2, 0)
        vec *= cls(0, 3, 0)
        assert vec.value == (0.0, 6.0, 0.0)

        if( cls == pimath.V3i ):
            assert (cls(0, 2, 0) * 3).value == (0.0, 6.0, 0.0)
        else:
            assert (cls(0, 2, 0) * 3.0).value == (0.0, 6.0, 0.0)

        vec.negate()
        assert vec.value == (-0.0, -6.0, -0.0)
        assert (-vec).value == (0.0, 6.0, 0.0)

        vec.normalize()
        assert vec.value == (-0.0, -1.0, -0.0)
        vec.x = 3
        vec.y = 0
        vec.z = 0

        assert vec.length() == 3.0

        assert vec.normalized().value == (1.0, 0.0, 0.0)

    def testQuat(self):
        self.runQuatTest( pimath.Quatd, pimath.V3d )
        self.runQuatTest( pimath.Quatf, pimath.V3f )

    def runQuatTest(self, cls, vec):
        quat = cls(1.0, vec(0.0, 0.0, 0.0))
        assert quat.r == 1.0
        assert quat.v.value == (0.0, 0.0, 0.0)

        quat.r = 3.0
        quat.normalize()
        assert quat.length() == 1.0

    def testPrecision(self):
        x = pimath.V3f(0.1, 0.0, 0.8)
        x.normalize()

        assert x.length() - 1.0 < 0.00000000000000001

    def testEuler(self):
        self.runEulerTest( pimath.Eulerf, pimath.M33f )
        self.runEulerTest( pimath.Eulerd, pimath.M33d )

    def runEulerTest(self, cls, mat):
        euler = cls( math.pi, 0, 0 )
        matr = euler.toMatrix33()
        diff = matr - mat( ( (1,0,0), (0,-1,0), (0,0,-1) ) )
        for tup in diff:
            for ele in tup:
                assert ele < 0.0001 and ele > -0.0001

    def testBox2(self):
        self.runBox2Test( pimath.Box2i, pimath.V2i )
        self.runBox2Test( pimath.Box2f, pimath.V2f )
        self.runBox2Test( pimath.Box2d, pimath.V2d )
        self.runBox2Test( pimath.Box2h, pimath.V2h )

    def runBox2Test(self, cls, vec):
        box = cls( vec( 1, 2 ), vec( 3, 4 ) )
        assert box.value == ((1,2), (3,4))
        box.makeEmpty()
        assert box.min[0] > box.max[0]
        assert box.min[1] > box.max[1]
        box.extendBy( vec( 1,2 ) )
        assert box.value == ((1,2), (1,2))
        box.extendBy( vec( 2,1 ) )
        assert box.value == ((1,1), (2,2))
        if cls == pimath.Box2i:
            assert box.center().value == (1, 1)
        else:
            assert box.center().value == (1.5, 1.5)

    def testBox3(self):
        self.runBox3Test( pimath.Box3i, pimath.V3i )
        self.runBox3Test( pimath.Box3f, pimath.V3f )
        self.runBox3Test( pimath.Box3d, pimath.V3d )
        self.runBox3Test( pimath.Box3h, pimath.V3h )

    def runBox3Test(self, cls, vec):
        box = cls( vec( 1, 2, 3 ), vec( 3, 4, 5 ) )
        assert box.value == ((1,2,3), (3,4,5))
        box.makeEmpty()
        assert box.min[0] > box.max[0]
        assert box.min[1] > box.max[1]
        box.extendBy( vec( 1,2,3 ) )
        assert box.value == ((1,2,3), (1,2,3))
        box.extendBy( vec( 2,1,3 ) )
        assert box.value == ((1,1,3), (2,2,3))
        if cls == pimath.Box3i:
            assert box.center().value == (1, 1, 3)
        else:
            assert box.center().value == (1.5, 1.5, 3)

    def testBoxAlgo(self):
        self.runBoxAlgoTest( pimath.Box2i, pimath.V2i, None, False, False )
        self.runBoxAlgoTest( pimath.Box2f, pimath.V2f, None, False, True )
        self.runBoxAlgoTest( pimath.Box2d, pimath.V2d, None, False, True )
        self.runBoxAlgoTest( pimath.Box2h, pimath.V2h, None, False, True )
        self.runBoxAlgoTest( pimath.Box3i, pimath.V3i, None, True, False )
        self.runBoxAlgoTest( pimath.Box3f, pimath.V3f, pimath.M44f, True, True )
        self.runBoxAlgoTest( pimath.Box3d, pimath.V3d, pimath.M44d, True, True )
        #V3h does not support the floating-point specific BoxAlgo functions
        self.runBoxAlgoTest( pimath.Box3h, pimath.V3h, None, True, True )

    def runBoxAlgoTest(self, cls, vec, transformType, threeD, floating ):
        v1 = vec()
        v2 = vec()
        v1.x = 0
        v1.y = 0
        v2.x = 2
        v2.y = 2
        if threeD:
            v1.z = 0
            v2.z = 2
        box = cls( v1, v2 )
        v3 = vec()
        if( threeD ):
            v3.value = ( 1, 3, 1 )
        else:
            v3.value = ( 1, 3 )
        v4 = pimath.clip( v3, box )
        if( threeD ):
            assert v4 == vec( 1, 2, 1 )
        else:
            assert v4 == vec( 1, 2 )
        if floating and threeD and transformType != None:
            transform = transformType( ( ( 1, 0, 0, 0 ), ( 0, 1, 0, 0 ), ( 0, 0, 1, 0 ), ( 1, 1, 1, 1 ) ) )
            newBox = pimath.transform( box, transform )
            assert newBox == cls( vec( 1, 1, 1 ), vec( 3, 3, 3 ) )

    def testColor(self):
        #Color3c is disabled because it requires compiling Vec3c, which warns.
        #self.runColor3Test( pimath.C3c )
        self.runColor3Test( pimath.C3f )
        self.runColor3Test( pimath.C3h )
        self.runColor4Test( pimath.C4c )
        self.runColor4Test( pimath.C4f )
        self.runColor4Test( pimath.C4h )

    def runColor3Test(self, cls):
        color = cls( 1, 0, 0 )
        hsv = pimath.rgb2hsv( color )
        maxV = 1
        assert hsv == cls( 0, maxV, 1 )
        assert pimath.hsv2rgb( hsv ) == color
        assert pimath.rgb2packed( cls( maxV,maxV,maxV ) ) == 256*256*256*256-1

    def runColor4Test(self, cls):
        color = cls( 1, 0, 0, 0 )
        hsv = pimath.rgb2hsv( color )
        maxV = 1
        if cls == pimath.C4c:
            maxV = 255
        assert hsv == cls( 0, maxV, 1, 0 )
        assert pimath.hsv2rgb( hsv ) == color
        assert pimath.rgb2packed( cls( maxV,maxV,maxV,maxV ) ) == 256*256*256*256-1

    def testFrame(self):
        self.runFrameTest( pimath.V3f, pimath.M44f )
        self.runFrameTest( pimath.V3d, pimath.M44d )
        self.runFrameTest( pimath.V3h, pimath.M44h )

    def runFrameTest(self, vec, mat):
        frame = pimath.firstFrame( vec( 0,0,0 ), vec( 0,0,1 ), vec( 0,0,2 ) )
        assert frame.value == ( (0.0, 0.0, 1.0, 0.0), (-1.0, 0.0, 0.0, 0.0), (0.0, -1.0, 0.0, 0.0), (0.0, 0.0, 0.0, 1.0))
        frame2 = pimath.nextFrame( frame, vec(0,0,2), vec(0,1,2), vec(0,0,1), vec(0,1,0) )
        targetMat = mat( ( (0,1,0,0), (-1,0,0,0), (0,0,1,0), (0,-1,2,1) ) )
        assert frame2.equalWithAbsError( targetMat, 0.001 )
        frame3 = pimath.lastFrame( frame2, vec(0,1,2), vec(0,2,2) )
        targetMat = mat( ( (0,1,0,0), (-1,0,0,0), (0,0,1,0), (0,0,2,1) ) )
        assert frame3.equalWithAbsError( targetMat, 0.001 )

    def testFrustum(self):
        self.runFrustumTest( pimath.Frustumf )
        self.runFrustumTest( pimath.Frustumd )

    def runFrustumTest(self, cls):
        frust = cls( 1,2,math.pi/2,0, 2 )
        assert near( frust.value, (1,2,-1,1,0.5,-0.5,False), 0.001 )
        assert abs( math.sin( frust.fovy() ) - 0.8 ) < 0.001

    def testInterval(self):
        self.runIntervalTest( pimath.Intervalf )
        self.runIntervalTest( pimath.Intervals )
        self.runIntervalTest( pimath.Intervald )
        self.runIntervalTest( pimath.Intervali )
        self.runIntervalTest( pimath.Intervalh )

    def runIntervalTest(self, cls):
        inter = cls( 1, 2 )
        inter.extendBy( 3 )
        assert inter.value == cls( ( 1, 3 ) ).value
        assert inter.size() == 2
        assert inter.center() == 2
        assert inter.intersects( cls( 2, 4 ) )

    def testLine(self):
        self.runLineTest( pimath.Line3f, pimath.V3f )
        self.runLineTest( pimath.Line3d, pimath.V3d )
        self.runLineTest( pimath.Line3h, pimath.V3h )

    def runLineTest(self, cls, vec ):
        line = cls( vec( 1, 1, 1 ), vec( 5.0/3, 5.0/3, 4.0/3 ) )
        point = line.closestPointTo( vec( 1,1,2) )
        assert point.equalWithAbsError( vec( 1.2222, 1.2222, 1.1111 ), 0.001 )

    def testLineAlgo(self):
        self.runLineAlgoTest( pimath.Line3f, pimath.V3f )
        self.runLineAlgoTest( pimath.Line3d, pimath.V3d )
        self.runLineAlgoTest( pimath.Line3h, pimath.V3h )

    def runLineAlgoTest(self, cls, vec ):
        line1 = cls( vec( 1, 1, 1 ), vec( 1, 1, 3 ) )
        line2 = cls( vec( 2, 0, 2 ), vec( 2, 2, 2 ) )
        points = pimath.closestPoints( line1, line2 )
        assert points[0] == vec( 1, 1, 2 )
        assert points[1] == vec( 2, 1, 2 )
        point = vec( 0, 0, 0 )
        assert pimath.rotatePoint( point, line1, math.pi ).equalWithAbsError( vec( 2, 2, 0 ), 0.001 )

    def testPlane(self):
        self.runPlaneTest( pimath.Plane3f, pimath.V3f, pimath.Line3f )
        self.runPlaneTest( pimath.Plane3d, pimath.V3d, pimath.Line3d )
        self.runPlaneTest( pimath.Plane3h, pimath.V3h, pimath.Line3h )

    def runPlaneTest(self, cls, vec, lineT):
        plane = cls( vec( 1, 1, 1 ), vec( 0, 1, 0 ) )
        assert plane.value == ((0,1,0),1)
        line = lineT( vec( 0, 2, 0 ), vec( 2, 0, 0 ) )
        assert near( plane.intersect( line ).value, ( 1, 1, 0 ), 0.001 )
        line2 = lineT( vec( 0, 2, 0 ), vec( 2, 2, 2 ) )
        assert plane.intersect( line2 ) == None
        assert plane.distanceTo( vec( 0,0,0) ) == -1

    def testRandom(self):
        self.runRandomTest( pimath.Rand32 )
        self.runRandomTest( pimath.Rand48 )

    def runRandomTest(self, randT):
        rand = randT()
        vec = pimath.solidSphereRand3f( rand )
        assert vec.length() < 1

    def testRoots(self):
        assert pimath.solveLinear( 1, 1 ) == (-1,)
        assert pimath.solveQuadratic( 1,1,1 ) == ()
        assert pimath.solveQuadratic( 1,0,-1 ) == (1,-1)
        assert near( pimath.solveCubic( 1,1,1,1 ), (-1,), 0.001 )
        assert near( pimath.solveCubic( 1,0,-1,0), (1,-1,0), 0.001)

    def testShear(self):
        self.runShearTest( pimath.Shear6f )
        self.runShearTest( pimath.Shear6d )

    def runShearTest(self, cls):
        shear = cls( 1, 1, 1, 0, 0, 0 )
        assert str( shear ) == "(1 1 1 0 0 0)"
        assert shear.value == (1,1,1,0,0,0)

    def testSphere(self):
        self.runSphereTest( pimath.Sphere3f, pimath.Line3f, pimath.V3f )
        self.runSphereTest( pimath.Sphere3d, pimath.Line3d, pimath.V3d )
        self.runSphereTest( pimath.Sphere3h, pimath.Line3h, pimath.V3h )

    def runSphereTest(self, cls, line, vec):
        sphere = cls( ( (1,2,2), 3 ) )
        line1 = line( vec( 0,-1,0 ), vec( 0,1,0) )
        line2 = line( vec( -10,-1,0 ), vec( -10,1,0) )
        assert sphere.intersect(line1).value == (0,0,0)
        assert sphere.intersectT(line1) == 1
        assert sphere.intersect(line2) == None
        assert sphere.intersectT(line2) == None

    def testVecAlgo(self):
        self.runVecAlgoTest2(pimath.V2f)
        self.runVecAlgoTest2(pimath.V2d)
        self.runVecAlgoTest2(pimath.V2h)

        self.runVecAlgoTest3(pimath.V3f)
        self.runVecAlgoTest3(pimath.V3d)
        self.runVecAlgoTest3(pimath.V3h)

        self.runVecAlgoTest4(pimath.V4f)
        self.runVecAlgoTest4(pimath.V4d)
        self.runVecAlgoTest4(pimath.V4h)

    def runVecAlgoTest2(self, cls):
        vec = cls(1,1)
        vec2 = cls(1,0)
        vec3 = cls(0,0)
        vec4 = cls(0,0.5)
        assert near( pimath.project( vec, vec2 ).value, ( 0.5, 0.5 ), 0.001 )
        assert near( pimath.project( vec2, vec ).value, ( 1.0, 0.0 ), 0.001 )
        assert near( pimath.orthogonal( vec, vec2 ).value, (0.5,-0.5), 0.001 )
        assert near( pimath.orthogonal( vec2, vec ).value, (0.0, 1.0), 0.001 )
        assert near( pimath.reflect( vec, vec2 ).value, (1.0,-1.0), 0.001 )
        assert near( pimath.reflect( vec2, vec ).value, (0.0, 1.0), 0.001 )
        assert near( pimath.closestVertex( vec2, vec3, vec4, vec ).value, (1,0), 0.001 )

    def runVecAlgoTest3(self, cls):
        vec = cls(1,1,1)
        vec2 = cls(1,0,0)
        assert near( pimath.project( vec, vec2 ).value, ( 0.3333, 0.3333, 0.3333 ), 0.001 )
        assert near( pimath.project( vec2, vec ).value, ( 1.0, 0.0, 0.0 ), 0.001 )
        assert near( pimath.orthogonal( vec, vec2 ).value, (0.6666,-0.3333, -0.3333), 0.001 )
        assert near( pimath.orthogonal( vec2, vec ).value, (0.0, 1.0, 1.0), 0.001 )
        assert near( pimath.reflect( vec, vec2 ).value, (1.0,-1.0, -1.0), 0.001 )
        assert near( pimath.reflect( vec2, vec ).value, (-0.3333, 0.6666, 0.6666), 0.001 )

    def runVecAlgoTest4(self, cls):
        vec = cls(1,1,1,1)
        vec2 = cls(1,0,0,0)
        assert near( pimath.project( vec, vec2 ).value, ( 0.25, 0.25, 0.25, 0.25 ), 0.001 )
        assert near( pimath.project( vec2, vec ).value, ( 1.0, 0.0, 0.0, 0.0 ), 0.001 )
        assert near( pimath.orthogonal( vec, vec2 ).value, (0.75,-0.25, -0.25, -0.25), 0.001 )
        assert near( pimath.orthogonal( vec2, vec ).value, (0.0, 1.0, 1.0, 1.0), 0.001 )
        assert near( pimath.reflect( vec, vec2 ).value, (1.0,-1.0,-1.0,-1.0), 0.001 )
        assert near( pimath.reflect( vec2, vec ).value, (-0.5, 0.5, 0.5, 0.5), 0.001 )

    def runTest(self):
        self.testMatrix44( )
        self.testMatrix33( )
        self.testVector3( )
        self.testQuat( )
        self.testPrecision( )
        self.testEuler( )
        self.testBox2( )
        self.testBox3( )
        self.testBoxAlgo( )
        self.testColor( )
        self.testFrustum( )
        self.testInterval( )
        self.testLine( )
        self.testLineAlgo( )
        self.testPlane( )
        self.testRandom( )
        self.testRoots( )
        self.testShear( )
        self.testSphere( )
        self.testVecAlgo( )
        pass


if __name__ == '__main__':
    unittest.main()
    test = TestPimath()
    test.runTest()


