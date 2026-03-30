    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r6
    stw r30, 0x18(r1)
    mr r30, r5
    stw r29, 0x14(r1)
    mr r29, r4
    bne @L_80134E48
    li r3, 0x0
    li r4, 0x3
    bl fn_80129280
    @L_80134E48:
    extsb. r0, r30
    blt @L_80134E5C
    extsb r4, r30
    cmpwi r4, 0x3
    blt @L_80134E64
    @L_80134E5C:
    li r6, 0x0
    b @L_80134E94
    @L_80134E64:
    extsb. r0, r31
    blt @L_80134E78
    extsb r0, r31
    cmpwi r0, 0x1e
    blt @L_80134E80
    @L_80134E78:
    li r6, 0x0
    b @L_80134E94
    @L_80134E80:
    mulli r4, r4, 0x24a4
    mulli r0, r0, 0x138
    add r4, r4, r0
    addi r6, r4, 0x14
    add r6, r3, r6
    @L_80134E94:
    cmplwi r6, 0x0
    bne @L_80134EA4
    li r3, 0x0
    b @L_80134ED4
    @L_80134EA4:
    li r0, 0x27
    subi r5, r6, 0x4
    subi r4, r29, 0x4
    mtctr r0
    @L_80134EB4:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_80134EB4
    mr r3, r6
    bl fn_8012086C
    li r3, 0x1
    @L_80134ED4:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
