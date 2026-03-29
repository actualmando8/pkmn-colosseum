    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r3
    bl fn_80103FE4
    mr r0, r3
    mr r3, r28
    mr r30, r0
    bl fn_801040A0
    mr r31, r3
    lwz r3, 0x40(r30)
    bl fn_80205B8C
    mr r29, r3
    bl fn_80105624
    lbz r0, 0x0(r31)
    lhz r3, 0x4(r3)
    cmpwi r0, 0x1
    beq @L_80010908
    bge @L_8001097C
    cmpwi r0, 0x0
    bge @L_800108AC
    b @L_8001097C
    @L_800108AC:
    cmplwi r29, 0x0
    beq @L_800108FC
    lbz r6, 0x95(r28)
    mr r3, r29
    li r4, 0x0
    li r5, 0x7f
    extsb r6, r6
    bl fn_8012640C
    clrlwi r0, r3, 16
    cmplwi r0, 0x165
    bne @L_800108FC
    bl fn_80105624
    lhz r0, 0x4(r3)
    rlwinm r0, r0, 0, 26, 26
    cmpwi r0, 0x0
    beq @L_8001097C
    li r0, 0x1
    stb r0, 0x98(r28)
    stb r0, 0x99(r28)
    b @L_8001097C
    @L_800108FC:
    mr r3, r28
    bl fn_80102ED4
    b @L_8001097C
    @L_80010908:
    clrlwi r3, r3, 16
    andi. r0, r3, 0xd0
    cmpwi r0, 0x0
    beq @L_80010960
    cmplwi r29, 0x0
    beq @L_8001094C
    lbz r4, 0x2(r31)
    mr r3, r29
    lbz r5, 0x95(r28)
    extsb r4, r4
    extsb r5, r5
    bl fn_80123C54
    lwz r28, 0x40(r30)
    mr r3, r29
    mr r4, r30
    bl fn_801204A8
    stw r28, 0x40(r30)
    @L_8001094C:
    li r3, -0x1
    li r0, 0x0
    stb r3, 0x2(r31)
    stb r0, 0x0(r31)
    b @L_8001097C
    @L_80010960:
    rlwinm r0, r3, 0, 26, 26
    cmpwi r0, 0x0
    beq @L_8001097C
    li r3, -0x1
    li r0, 0x0
    stb r3, 0x2(r31)
    stb r0, 0x0(r31)
    @L_8001097C:
    li r3, 0x0
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
