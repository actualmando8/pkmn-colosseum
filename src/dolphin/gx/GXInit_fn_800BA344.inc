    nofralloc
    lfs f0, -29576(r2)
    fcmpo cr0, f1, f0
    bge @L_800BA354
    li r4, 0x0
@L_800BA354:
    lfs f0, -29576(r2)
    fcmpo cr0, f2, f0
    cror eq, lt, eq
    beq @L_800BA374
    lfs f0, -29552(r2)
    fcmpo cr0, f2, f0
    cror eq, gt, eq
    bne @L_800BA378
@L_800BA374:
    li r4, 0x0
@L_800BA378:
    cmpwi r4, 0x2
    beq @L_800BA3B8
    bge @L_800BA394
    cmpwi r4, 0x0
    beq @L_800BA3F8
    bge @L_800BA3A0
    b @L_800BA3F8
@L_800BA394:
    cmpwi r4, 0x4
    bge @L_800BA3F8
    b @L_800BA3DC
@L_800BA3A0:
    lfs f5, -29552(r2)
    fmuls f0, f2, f1
    lfs f4, -29576(r2)
    fsubs f1, f5, f2
    fdivs f3, f1, f0
    b @L_800BA404
@L_800BA3B8:
    lfs f5, -29552(r2)
    fmuls f4, f2, f1
    lfs f3, -29532(r2)
    fsubs f2, f5, f2
    fmuls f0, f1, f4
    fmuls f1, f3, f2
    fdivs f3, f1, f4
    fdivs f4, f1, f0
    b @L_800BA404
@L_800BA3DC:
    fmuls f0, f2, f1
    lfs f5, -29552(r2)
    lfs f3, -29576(r2)
    fsubs f2, f5, f2
    fmuls f0, f1, f0
    fdivs f4, f2, f0
    b @L_800BA404
@L_800BA3F8:
    lfs f3, -29576(r2)
    lfs f5, -29552(r2)
    fmr f4, f3
@L_800BA404:
    stfs f5, 0x1c(r3)
    stfs f3, 0x20(r3)
    stfs f4, 0x24(r3)
    blr
