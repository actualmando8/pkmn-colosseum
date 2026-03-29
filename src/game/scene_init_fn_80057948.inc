    nofralloc
    lis r3, lbl_803A9768@ha
    lfs f2, lbl_8047BF00(r2)
    addi r4, r3, lbl_803A9768@l
    lfs f3, 0x288(r4)
    fcmpo cr0, f3, f2
    ble @L_800579B8
    lfs f1, 0x284(r4)
    lfs f0, lbl_8047BEF4(r2)
    fadds f1, f1, f3
    fcmpo cr0, f1, f0
    stfs f1, 0x284(r4)
    cror eq, gt, eq
    bne @L_80057984
    stfs f0, 0x284(r4)
    stfs f2, 0x288(r4)
    @L_80057984:
    lis r3, lbl_803A9768@ha
    lfs f2, 0x284(r4)
    addi r3, r3, lbl_803A9768@l
    lfs f3, 0x28c(r3)
    lfs f1, 0x294(r3)
    lfs f4, 0x290(r3)
    lfs f0, 0x298(r3)
    fsubs f1, f1, f3
    fsubs f0, f0, f4
    fmadds f1, f2, f1, f3
    fmadds f0, f2, f0, f4
    stfs f1, 0x27c(r3)
    stfs f0, 0x280(r3)
    @L_800579B8:
    lfs f2, lbl_8047A58C(r13)
    lfs f1, lbl_8047BF0C(r2)
    lfs f0, lbl_8047BEF4(r2)
    fadds f1, f2, f1
    fcmpo cr0, f1, f0
    stfs f1, lbl_8047A58C(r13)
    ble @L_800579DC
    fsubs f0, f1, f0
    stfs f0, lbl_8047A58C(r13)
    @L_800579DC:
    lfs f2, lbl_8047A588(r13)
    lfs f1, lbl_8047BEF4(r2)
    fcmpo cr0, f2, f1
    bgelr
    lfs f0, lbl_8047BF0C(r2)
    fadds f0, f2, f0
    fcmpo cr0, f0, f1
    stfs f0, lbl_8047A588(r13)
    blelr
    stfs f1, lbl_8047A588(r13)
    blr
