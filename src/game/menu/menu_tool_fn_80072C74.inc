    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    stw r29, 0x24(r1)
    mr r29, r3
    mr r30, r4
    bl fn_800D0F44
    subis r0, r3, 0x4
    cmplwi r0, 0x0
    beq @L_80072CAC
    li r31, 0x1
    b @L_80072D24
@L_80072CAC:
    mr r3, r29
    addi r4, r1, 0x8
    bl fn_8025F3F4
    cmpwi r3, 0x0
    beq @L_80072CC8
    li r31, 0x2
    b @L_80072D24
@L_80072CC8:
    lbz r0, 0x8(r1)
    rlwinm r0, r0, 0, 28, 28
    cmpwi r0, 0x0
    bne @L_80072CF8
    li r0, 0x11
    mr r3, r29
    stw r0, 0xc(r1)
    addi r4, r1, 0xc
    addi r5, r1, 0x8
    bl fn_8025F648
    li r31, -0x1
    b @L_80072D24
@L_80072CF8:
    mr r3, r29
    addi r4, r1, 0x10
    addi r5, r1, 0x8
    bl fn_8025F584
    cmpwi r3, 0x0
    beq @L_80072D18
    li r31, 0x3
    b @L_80072D24
@L_80072D18:
    lwz r0, 0x10(r1)
    li r31, 0x0
    stw r0, 0x0(r30)
@L_80072D24:
    cmpwi r31, 0x0
    blt @L_80072D38
    addi r3, r29, 0x1
    li r4, 0x1
    bl fn_8008ABE4
@L_80072D38:
    mr r3, r31
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r29, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
