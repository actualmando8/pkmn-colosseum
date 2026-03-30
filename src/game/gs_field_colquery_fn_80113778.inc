    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r30, r3
    mr r31, r4
    cmplwi r30, 0x0
    beq @L_80113810
    lis r3, lbl_80408378@ha
    addi r3, r3, lbl_80408378@l
    addi r3, r3, 0x10
    bl fn_800E01F4
    extsh r3, r31
    lis r0, 0x4330
    xoris r3, r3, 0x8000
    lfs f1, -26416(r2)
    stw r3, 0xc(r1)
    lis r3, lbl_80408378@ha
    addi r3, r3, lbl_80408378@l
    lfd f2, -26400(r2)
    stw r0, 0x8(r1)
    fmr f3, f1
    lfs f4, -26412(r2)
    addi r3, r3, 0x1c
    lfd f0, 0x8(r1)
    fsubs f0, f0, f2
    fmuls f2, f4, f0
    bl fn_800E01F4
    bl fn_800FF56C
    lis r4, lbl_80408378@ha
    li r0, 0x0
    addi r4, r4, lbl_80408378@l
    stw r3, 0x0(r4)
    mr r3, r30
    stw r30, 0x4(r4)
    stb r0, 0x8(r4)
    bl fn_800FF58C
@L_80113810:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

