    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r30, 0x28(r1)
    bl fn_801337A0
    mr r0, r3
    li r3, 0x1
    mr r30, r0
    bl fn_801337A8
    bl fn_8000816C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_801EF99C
    li r3, 0x1
    bl fn_80133810
    b @L_801EF9EC
@L_801EF99C:
    li r31, 0x0
    b @L_801EF9CC
@L_801EF9A4:
    addi r3, r1, 0x8
    li r4, 0x1
    bl fn_80103BA8
    lhz r0, 0x8(r1)
    rlwinm. r0, r0, 0, 20, 20
    bne @L_801EF9D8
    bl fn_800F0308
    bl fn_800D3088
    add r0, r31, r3
    rlwinm r31, r0, 0, 24, 31
@L_801EF9CC:
    rlwinm r0, r31, 0, 24, 31
    cmplwi r0, 0x4
    blt @L_801EF9A4
@L_801EF9D8:
    rlwinm r0, r31, 0, 24, 31
    cmplwi r0, 0x4
    bge @L_801EF9EC
    li r3, 0x1
    bl fn_80133810
@L_801EF9EC:
    mr r3, r30
    bl fn_801337A8
    lmw r30, 0x28(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
