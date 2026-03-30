    nofralloc
/* 800DFEEC 000DCEEC  C0 C5 00 04 */	lfs f6, 0x4(r5)
/* 800DFEF0 000DCEF0  C1 84 00 04 */	lfs f12, 0x4(r4)
/* 800DFEF4 000DCEF4  C1 45 00 08 */	lfs f10, 0x8(r5)
/* 800DFEF8 000DCEF8  C1 64 00 00 */	lfs f11, 0x0(r4)
/* 800DFEFC 000DCEFC  EC 06 03 32 */	fmuls f0, f6, f12
/* 800DFF00 000DCF00  C0 A5 00 00 */	lfs f5, 0x0(r5)
/* 800DFF04 000DCF04  EC 4A 03 32 */	fmuls f2, f10, f12
/* 800DFF08 000DCF08  C1 04 00 08 */	lfs f8, 0x8(r4)
/* 800DFF0C 000DCF0C  EC 26 02 F2 */	fmuls f1, f6, f11
/* 800DFF10 000DCF10  C1 24 00 0C */	lfs f9, 0xc(r4)
/* 800DFF14 000DCF14  EC 05 02 FA */	fmadds f0, f5, f11, f0
/* 800DFF18 000DCF18  EC 65 12 7A */	fmadds f3, f5, f9, f2
/* 800DFF1C 000DCF1C  EC 45 02 32 */	fmuls f2, f5, f8
/* 800DFF20 000DCF20  EC EA 02 3A */	fmadds f7, f10, f8, f0
/* 800DFF24 000DCF24  EC 2A 0A 7A */	fmadds f1, f10, f9, f1
/* 800DFF28 000DCF28  EC 86 1A 3C */	fnmsubs f4, f6, f8, f3
/* 800DFF2C 000DCF2C  EC 0B 01 F2 */	fmuls f0, f11, f7
/* 800DFF30 000DCF30  EC 46 12 7A */	fmadds f2, f6, f9, f2
/* 800DFF34 000DCF34  EC C5 0B 3C */	fnmsubs f6, f5, f12, f1
/* 800DFF38 000DCF38  EC 09 01 3A */	fmadds f0, f9, f4, f0
/* 800DFF3C 000DCF3C  EC AA 12 FC */	fnmsubs f5, f10, f11, f2
/* 800DFF40 000DCF40  EC 0C 01 BA */	fmadds f0, f12, f6, f0
/* 800DFF44 000DCF44  EC 08 01 7C */	fnmsubs f0, f8, f5, f0
/* 800DFF48 000DCF48  D0 03 00 00 */	stfs f0, 0x0(r3)
/* 800DFF4C 000DCF4C  C0 04 00 04 */	lfs f0, 0x4(r4)
/* 800DFF50 000DCF50  C0 24 00 0C */	lfs f1, 0xc(r4)
/* 800DFF54 000DCF54  EC 00 01 F2 */	fmuls f0, f0, f7
/* 800DFF58 000DCF58  C0 44 00 08 */	lfs f2, 0x8(r4)
/* 800DFF5C 000DCF5C  C0 64 00 00 */	lfs f3, 0x0(r4)
/* 800DFF60 000DCF60  EC 01 01 7A */	fmadds f0, f1, f5, f0
/* 800DFF64 000DCF64  EC 02 01 3A */	fmadds f0, f2, f4, f0
/* 800DFF68 000DCF68  EC 03 01 BC */	fnmsubs f0, f3, f6, f0
/* 800DFF6C 000DCF6C  D0 03 00 04 */	stfs f0, 0x4(r3)
/* 800DFF70 000DCF70  C0 04 00 08 */	lfs f0, 0x8(r4)
/* 800DFF74 000DCF74  C0 24 00 0C */	lfs f1, 0xc(r4)
/* 800DFF78 000DCF78  EC 00 01 F2 */	fmuls f0, f0, f7
/* 800DFF7C 000DCF7C  C0 44 00 00 */	lfs f2, 0x0(r4)
/* 800DFF80 000DCF80  C0 64 00 04 */	lfs f3, 0x4(r4)
/* 800DFF84 000DCF84  EC 01 01 BA */	fmadds f0, f1, f6, f0
/* 800DFF88 000DCF88  EC 02 01 7A */	fmadds f0, f2, f5, f0
/* 800DFF8C 000DCF8C  EC 03 01 3C */	fnmsubs f0, f3, f4, f0
/* 800DFF90 000DCF90  D0 03 00 08 */	stfs f0, 0x8(r3)
/* 800DFF94 000DCF94  4E 80 00 20 */	blr
