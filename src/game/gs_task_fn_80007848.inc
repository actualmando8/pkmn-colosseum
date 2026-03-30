    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    lis r3, 0x8001
    addi r31, r3, -0x7DBC
    @L_80007860:
    lhz r3, lbl_8047A28A(r13)
    mr r5, r31
    addi r4, r1, 0x8
    bl fn_8001E304
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80007884
    li r0, -0x1
    b @L_800078A8
    @L_80007884:
    lwz r3, lbl_80478F28(r13)
    lwz r0, 0x8(r1)
    lwz r3, 0x0(r3)
    cmplw r0, r3
    ble @L_800078A0
    subi r0, r3, 0x1
    stw r0, 0x8(r1)
    @L_800078A0:
    bl fn_8001E200
    lwz r0, 0x8(r1)
    @L_800078A8:
    cmpwi r0, 0x0
    bge @L_800078B8
    li r3, 0x1
    b @L_800078D8
    @L_800078B8:
    clrlwi r3, r0, 16
    li r4, 0x3e
    sth r3, lbl_8047A28A(r13)
    li r5, 0x0
    li r6, 0xc8
    li r7, -0xc8
    bl fn_80053064
    b @L_80007860
    @L_800078D8:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
