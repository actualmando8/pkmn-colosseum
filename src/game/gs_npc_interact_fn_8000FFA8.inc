    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r3
    mr r29, r4
    li r31, 0x0
    li r30, -0x1
    bl fn_801040A0
    lha r4, 0x6(r29)
    subi r0, r4, 0x11aa
    cmplwi r0, 0x17
    bgt @L_8001002C
    lis r4, jumptable_802E4D2C@ha
    slwi r0, r0, 2
    addi r4, r4, jumptable_802E4D2C@l
    lwzx r0, r4, r0
    mtctr r0
    bctr
    li r30, 0x0
    b @L_8001002C
    li r30, 0x1
    b @L_8001002C
    li r30, 0x2
    b @L_8001002C
    li r30, 0x3
    b @L_8001002C
    li r30, 0x4
    b @L_8001002C
    li r30, 0x5
    @L_8001002C:
    cmpwi r30, 0x0
    blt @L_800100A0
    lbzx r0, r3, r30
    cmpwi r0, 0x2
    beq @L_80010070
    bge @L_80010054
    cmpwi r0, 0x0
    beq @L_80010060
    bge @L_80010068
    b @L_8001007C
    @L_80010054:
    cmpwi r0, 0x4
    bge @L_8001007C
    b @L_80010078
    @L_80010060:
    li r31, 0x1b4
    b @L_8001007C
    @L_80010068:
    li r31, 0x3ac
    b @L_8001007C
    @L_80010070:
    li r31, 0x3ae
    b @L_8001007C
    @L_80010078:
    li r31, 0x3ad
    @L_8001007C:
    clrlwi r0, r31, 16
    cmplwi r0, 0x0
    beq @L_800100A0
    mr r5, r28
    mr r6, r31
    li r3, 0x0
    li r4, 0x0
    li r7, 0x0
    bl fn_801040F0
    @L_800100A0:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
