#include "common.h"
#include "control.h"

int main()
{
	//set GBA rendering context to MODE 3 Bitmap Rendering
	REG_DISPCNT = DCNT_MODE3 | DCNT_BG2;

	irq_init(NULL);
	// Maxmod requires the vblank interrupt to reset sound DMA.
	// Link the VBlank interrupt to mmVBlank, and enable it. 
	irq_add( II_VBLANK, mmVBlank );
	irq_enable(II_VBLANK);

	gba_seed_rand(14);
	CTL_init();

	while (1) {
		//wait until v-sync occurs
		VBlankIntrWait();
		mmFrame();
		
		CTL_run();
	}
	
	return 0;
}
