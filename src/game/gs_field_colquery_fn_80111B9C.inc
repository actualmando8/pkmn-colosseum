    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r5
    stw r30, 0x18(r1)
    mr r30, r4
    stw r29, 0x14(r1)
    mr r29, r3
    bl fn_8010CBC0
    cmplwi r3, 0x0
    bne @L_80111BD4
    li r3, 0x0
    b @L_80111C08
@L_80111BD4:
    mr r3, r30
    mr r4, r29
    bl fn_800A3C00
    lfs f0, -26432(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_80111BF8
    li r3, 0x0
    b @L_80111C08
@L_80111BF8:
    mr r3, r29
    mr r4, r30
    mr r5, r31
    bl fn_80111864
@L_80111C08:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

