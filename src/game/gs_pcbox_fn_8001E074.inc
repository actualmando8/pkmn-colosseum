    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    stw r6, 0x8(r1)
    mr r28, r4
    mr r29, r5
    cmpwi r6, 0x0
    beq @L_8001E0AC
    li r0, 0x1
    stw r0, 0x8(r1)
    @L_8001E0AC:
    clrlwi r0, r3, 24
    cmpwi r0, 0x1
    beq @L_8001E0DC
    bge @L_8001E0C8
    cmpwi r0, 0x0
    bge @L_8001E0D4
    b @L_8001E0E4
    @L_8001E0C8:
    cmpwi r0, 0x7f
    beq @L_8001E0E4
    b @L_8001E0E4
    @L_8001E0D4:
    li r30, 0x11
    b @L_8001E0E8
    @L_8001E0DC:
    li r30, 0x12
    b @L_8001E0E8
    @L_8001E0E4:
    li r30, 0x44
    @L_8001E0E8:
    bl fn_801046B8
    extsh r31, r30
    mr r4, r3
    mr r3, r31
    addi r5, r1, 0x8
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    crxor 6, 6, 6
    bl fn_801026A4
    extsh r0, r28
    cmpwi r0, 0x0
    blt @L_8001E138
    extsh r0, r29
    cmpwi r0, 0x0
    blt @L_8001E138
    mr r3, r31
    mr r4, r28
    mr r5, r29
    bl fn_80102868
    @L_8001E138:
    mr r3, r31
    li r4, 0x1
    bl fn_801045A8
    mr r3, r31
    bl fn_801043A4
    extsb r30, r3
    mr r3, r31
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    mr r3, r30
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
