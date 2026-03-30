    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r3
    addi r6, r31, 0x32
    stw r30, 0x18(r1)
    mr r30, r4
    addi r4, r31, 0x1a
    stw r29, 0x14(r1)
    mr r29, r5
    lbz r3, 0x19(r3)
    lbz r5, 0x40(r31)
    bl fn_800B9C44
    lwz r3, 0x44(r31)
    bl fn_800B9E6C
    lbz r3, 0x50(r31)
    bl fn_801B278C
    lbz r3, 0x51(r31)
    bl fn_801B273C
    lbz r3, 0x52(r31)
    li r4, 0x3
    li r5, 0x1
    bl fn_801B27DC
    lwz r0, 0x48(r31)
    addi r3, r1, 0x8
    stw r0, 0x8(r1)
    lwz r4, 0x4c(r31)
    bl fn_800B9BDC
    cmpwi r29, 0x1
    beq @L_801BFB4C
    bge @L_801BFAA8
    cmpwi r29, 0x0
    bge @L_801BFAB4
    b @L_801BFC6C
@L_801BFAA8:
    cmpwi r29, 0x3
    bge @L_801BFC6C
    b @L_801BFBBC
@L_801BFAB4:
    li r3, 0x3
    bl fn_800B9874
    lhz r5, 0x4(r31)
    li r3, 0x0
    lhz r6, 0x6(r31)
    li r4, 0x0
    bl fn_800B959C
    lhz r3, 0x6(r31)
    lhz r4, 0x8(r31)
    bl fn_800B98DC
    bl fn_800B9B14
    mr r0, r3
    lhz r3, 0x4(r31)
    mr r29, r0
    rlwinm r4, r0, 0, 16, 31
    bl fn_800B96BC
    mr r3, r30
    li r4, 0x1
    bl fn_800B9E88
    lhz r0, 0x8(r31)
    subf. r4, r29, r0
    beq @L_801BFC80
    lhz r3, 0x4(r31)
    addi r0, r3, 0xf
    rlwinm r0, r0, 1, 15, 26
    mullw r3, r0, r29
    mullw r0, r4, r0
    add r5, r30, r3
    rlwinm r4, r0, 30, 2, 31
    b @L_801BFB3C
@L_801BFB2C:
    lis r3, 0x1080
    addi r3, r3, 0x1080
    stw r3, 0x0(r5)
    addi r5, r5, 0x4
@L_801BFB3C:
    cmplwi r4, 0x0
    subi r4, r4, 0x1
    bne @L_801BFB2C
    b @L_801BFC80
@L_801BFB4C:
    lhz r6, 0x6(r31)
    li r3, 0x0
    lhz r5, 0x4(r31)
    li r4, 0x0
    subi r0, r6, 0x4
    rlwinm r6, r0, 0, 16, 31
    bl fn_800B959C
    lfs f1, -22376(r2)
    bl fn_800B9B14
    mr r0, r3
    lhz r3, 0x4(r31)
    rlwinm r4, r0, 0, 16, 31
    bl fn_800B96BC
    li r3, 0x1
    bl fn_800B9874
    lhz r6, 0x6(r31)
    li r3, 0x0
    lhz r5, 0x4(r31)
    li r4, 0x0
    subi r0, r6, 0x4
    rlwinm r29, r0, 0, 16, 31
    mr r6, r29
    bl fn_800B959C
    mr r3, r30
    li r4, 0x1
    bl fn_800B9E88
    bl fn_800B8E74
    b @L_801BFC84
@L_801BFBBC:
    lhz r6, 0x6(r31)
    li r3, 0x0
    lhz r5, 0x4(r31)
    li r4, 0x0
    subi r0, r6, 0x4
    rlwinm r6, r0, 0, 16, 31
    bl fn_800B959C
    lfs f1, -22376(r2)
    bl fn_800B9B14
    mr r0, r3
    lhz r3, 0x4(r31)
    rlwinm r4, r0, 0, 16, 31
    bl fn_800B96BC
    li r3, 0x2
    bl fn_800B9874
    lhz r6, 0x6(r31)
    li r3, 0x0
    lhz r5, 0x4(r31)
    li r4, 0x4
    subi r0, r6, 0x4
    rlwinm r29, r0, 0, 16, 31
    mr r6, r29
    bl fn_800B959C
    lhz r3, 0x4(r31)
    rlwinm r0, r29, 0, 16, 31
    li r4, 0x1
    addi r3, r3, 0xf
    rlwinm r3, r3, 0, 16, 27
    mullw r0, r3, r0
    rlwinm r0, r0, 1, 0, 30
    add r3, r30, r0
    bl fn_800B9E88
    lhz r5, 0x4(r31)
    li r3, 0x0
    li r4, 0x0
    li r6, 0x4
    bl fn_800B959C
    li r3, 0x3
    bl fn_800B9874
    lis r3, lbl_804657C0@ha
    li r4, 0x1
    addi r3, r3, lbl_804657C0@l
    bl fn_800B9E88
    b @L_801BFC80
@L_801BFC6C:
    lis r4, lbl_8027575C@ha
    addi r3, r2, -22384
    addi r5, r4, lbl_8027575C@l
    li r4, 0x219
    bl fn_80196D78
@L_801BFC80:
    bl fn_800B8E74
@L_801BFC84:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

