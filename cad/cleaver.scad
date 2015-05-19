$fn=64;

module board(xdim, ydim,zdim) {
	rdim =4;
	difference(){hull() {
		translate([rdim,rdim,0])cylinder(r=rdim, h=zdim);
		translate([xdim-rdim,rdim,0])cylinder(r=rdim, h=zdim);
		translate([rdim,ydim-rdim,0])cylinder(r=rdim, h=zdim);
		translate([xdim-rdim,ydim-rdim,0])cylinder(r=rdim, h=zdim);
	};

	translate([xdim-4,ydim-4,-0.1])cylinder(zdim*2,3.2/2,3.2/2);
	translate([xdim-4,4,-0.1])cylinder(zdim*2,3.2/2,3.2/2);
	translate([4,ydim-4,-0.1])cylinder(zdim*2,3.2/2,3.2/2);
	translate([4,4,-0.1])cylinder(zdim*2,3.2/2,3.2/2);
};
}

module led(x,y,z)
{
	color("green"){
		translate([x,y,z])cylinder(d=5, h=5);
		translate([x,y,z+5])sphere(d=5);
	}
}

module din_5(x,y,z)
{
	color("black")
	translate([x,y,z]) {
		cube([21,13.2,19.5]);
	}
}

module dc_jack(x,y,z)
{
	color("black")
	translate([x,y,z]) {
		cube([9,15.2,11]);
	}
}

module pot(x,y,z)
{
	color("grey") {
		translate([x,y,z]) {
			cube([9.8,11,6.8]);
			translate([9.8/2,11/2,0])cylinder(15,d=9);
		}
	}
}

module display(x,y,z)
{
	translate([x,y,z]) {
		translate([(23.5-17.5),0,0])color("lime")cube([17.5,36,4.1]);
		color("black")cube([(23.5-17.5),36,4.1]);
	}
}

module dpak3(x,y,z)
{
	color("black")
	translate([x,y,-4.5]) {
		cube([10.3,14.7,4.5]);
	}

}


module dpak3_turn(x,y,z)
{
	color("black")
	translate([x,y,-4.5]) {
		cube([14.7,10.3,4.5]);
		}

}

module cleaver()
{
	thickness=1.6;
	color("red")board(43,70,thickness);
	led(34,70-6.5,thickness);
	din_5(10.8,70-13.2,thickness);
	dc_jack(16.9,-3.5,thickness);
	pot(30,20,thickness);
	display(0,14, thickness);
	dpak3(3.8,46.4,thickness);
	dpak3_turn(10.9,16.9,thickness);
}

cleaver();