    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r30, r3
    mr r28, r4
    mr r29, r5
    mr r31, r6
    li r3, 0x1
    bl fn_80103CC0
    mr r0, r3
    mr r3, r30
    mr r30, r0
    mr r4, r28
    mr r5, r29
    mr r6, r31
    bl fn_80084A8C
    mr r31, r3
    li r3, 0x0
    bl fn_801069FC
    li r3, 0xe4
    bl fn_80102620
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80084A34
    li r3, 0xe4
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
@L_80084A34:
    mr r3, r30
    bl fn_80103CC0
    li r30, 0x0
@L_80084A40:
    mr r3, r30
    bl fn_80093698
    addi r30, r30, 0x1
    cmpwi r30, 0x3
    blt @L_80084A40
    rlwinm r0, r31, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80084A68
    li r3, 0x0
    b @L_80084A6C
@L_80084A68:
    li r3, -0x1
@L_80084A6C:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
