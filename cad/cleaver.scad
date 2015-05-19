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
	translate([x,y,z])cylinder(d=5, h=5);
	translate([x,y,z+5])sphere(d=5);
}
module pcb()
{
	color("red")board(43,70,1.6);
	color("green")led(33.8,6.5,1.6);
}

pcb();