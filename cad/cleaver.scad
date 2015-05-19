$fn=64;

module board(xdim, ydim) {
	zdim=1.6;
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

module pcb()
{
	board(43,70);
}

pcb();