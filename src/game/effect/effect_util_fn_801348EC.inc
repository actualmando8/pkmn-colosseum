    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    li r30, 0x0
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r4
    bne @L_80134924
    li r3, 0x0
    li r4, 0x3
    bl fn_80129280
    @L_80134924:
    extsb. r0, r28
    blt @L_80134938
    extsb r0, r28
    cmpwi r0, 0x3
    blt @L_80134940
    @L_80134938:
    li r3, -0x1
    b @L_801349BC
    @L_80134940:
    mulli r0, r0, 0x24a4
    li r29, 0x0
    add r31, r3, r0
    b @L_801349AC
    @L_80134950:
    extsb. r0, r28
    blt @L_80134964
    extsb r0, r28
    cmpwi r0, 0x3
    blt @L_8013496C
    @L_80134964:
    li r3, 0x0
    b @L_8013498C
    @L_8013496C:
    extsb. r0, r29
    blt @L_80134980
    extsb r0, r29
    cmpwi r0, 0x1e
    blt @L_80134988
    @L_80134980:
    li r3, 0x0
    b @L_8013498C
    @L_80134988:
    addi r3, r31, 0x14
    @L_8013498C:
    cmplwi r3, 0x0
    beq @L_801349A4
    bl fn_80123FBC
    clrlwi. r0, r3, 24
    beq @L_801349A4
    addi r30, r30, 0x1
    @L_801349A4:
    addi r31, r31, 0x138
    addi r29, r29, 0x1
    @L_801349AC:
    extsb r0, r29
    cmpwi r0, 0x1e
    blt @L_80134950
    mr r3, r30
    @L_801349BC:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
