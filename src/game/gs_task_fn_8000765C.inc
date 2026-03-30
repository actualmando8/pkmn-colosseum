    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    bl fn_801EF63C
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80007684
    li r3, -0x1
    b @L_800076F4
    @L_80007684:
    lis r3, fn_80008390@ha
    addi r31, r3, fn_80008390@l
    @L_8000768C:
    lhz r3, lbl_80478830(r13)
    mr r5, r31
    addi r4, r1, 0x8
    bl fn_8001E304
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_800076B0
    li r0, -0x1
    b @L_800076D4
    @L_800076B0:
    lwz r3, lbl_80478F08(r13)
    lwz r0, 0x8(r1)
    lwz r3, 0x0(r3)
    cmplw r0, r3
    blt @L_800076CC
    subi r0, r3, 0x1
    stw r0, 0x8(r1)
    @L_800076CC:
    bl fn_8001E200
    lwz r0, 0x8(r1)
    @L_800076D4:
    cmpwi r0, 0x0
    bge @L_800076E4
    li r3, 0x1
    b @L_800076F4
    @L_800076E4:
    clrlwi r3, r0, 16
    sth r3, lbl_80478830(r13)
    bl fn_80051710
    b @L_8000768C
    @L_800076F4:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
