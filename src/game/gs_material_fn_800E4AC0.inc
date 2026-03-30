    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r6
    stw r30, 0x18(r1)
    mr r30, r5
    stw r29, 0x14(r1)
    mr r29, r3
    lwz r0, 0x114(r3)
    cmpwi r0, 0x0
    bne @L_800E4BD8
    cmpwi r30, 0x0
    beq @L_800E4BD8
    lwz r3, 0x4(r4)
    clrlwi r0, r7, 24
    cmplwi r0, 0x1
    stw r3, 0x118(r29)
    lhz r0, 0x2(r4)
    stw r0, 0x11c(r29)
    bne @L_800E4B54
    lfs f1, lbl_8047CB7C(r2)
    addi r3, r29, 0x120
    fmr f2, f1
    fmr f3, f1
    bl fn_800E01F4
    lfs f1, lbl_8047CB7C(r2)
    addi r3, r29, 0x12c
    fmr f2, f1
    fmr f3, f1
    bl fn_800E01F4
    lfs f1, lbl_8047CB80(r2)
    addi r3, r29, 0x138
    fmr f2, f1
    fmr f3, f1
    bl fn_800E01F4
    b @L_800E4B78
    @L_800E4B54:
    addi r3, r29, 0x120
    addi r4, r29, 0x18
    bl fn_800E01D0
    addi r3, r29, 0x12c
    addi r4, r29, 0x24
    bl fn_800E01D0
    addi r3, r29, 0x138
    addi r4, r29, 0x30
    bl fn_800E01D0
    @L_800E4B78:
    lfs f1, lbl_8047CB7C(r2)
    addi r3, r29, 0x18
    fmr f2, f1
    fmr f3, f1
    bl fn_800E01F4
    lfs f1, lbl_8047CB7C(r2)
    addi r3, r29, 0x24
    fmr f2, f1
    fmr f3, f1
    bl fn_800E01F4
    lfs f1, lbl_8047CB80(r2)
    addi r3, r29, 0x30
    fmr f2, f1
    fmr f3, f1
    bl fn_800E01F4
    stw r30, 0x114(r29)
    clrlwi. r0, r31, 24
    lwz r0, 0x0(r29)
    oris r0, r0, 0x8
    stw r0, 0x0(r29)
    beq @L_800E4BD8
    lwz r0, 0x0(r29)
    oris r0, r0, 0x4
    stw r0, 0x0(r29)
    @L_800E4BD8:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
