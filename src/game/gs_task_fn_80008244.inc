    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r3
    lwz r3, lbl_80478F28(r13)
    lwz r0, 0x0(r3)
    cmplw r28, r0
    blt @L_8000828C
    lis r3, 0x1
    li r28, 0x0
    subi r31, r3, 0x149d
    mr r30, r31
    mr r29, r31
    b @L_800082EC
    @L_8000828C:
    clrlwi r4, r28, 16
    li r3, 0x0
    li r5, 0x40
    li r6, 0x0
    bl fn_801FB1C0
    mr r31, r3
    clrlwi r4, r28, 16
    li r3, 0x0
    li r5, 0x41
    li r6, 0x0
    bl fn_801FB1C0
    mr r30, r3
    clrlwi r4, r28, 16
    li r3, 0x0
    li r5, 0x3f
    li r6, 0x0
    bl fn_801FB1C0
    mr r29, r3
    clrlwi r4, r28, 16
    li r3, 0x0
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    mr r28, r3
    @L_800082EC:
    cmplwi r29, 0x0
    bne @L_800082FC
    lis r3, 0x1
    subi r29, r3, 0x149d
    @L_800082FC:
    cmplwi r31, 0x0
    bne @L_8000830C
    lis r3, 0x1
    subi r31, r3, 0x149d
    @L_8000830C:
    cmplwi r30, 0x0
    bne @L_8000831C
    lis r3, 0x1
    subi r30, r3, 0x149d
    @L_8000831C:
    mr r3, r31
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    mr r3, r30
    bl fn_800FA280
    mr r4, r3
    li r3, 0xe
    bl fn_80132A38
    mr r3, r29
    bl fn_800FA280
    mr r4, r3
    li r3, 0x4d
    bl fn_80132A38
    mr r4, r28
    li r3, 0x2f
    bl fn_80132A38
    lis r3, 0x1
    subi r3, r3, 0xea7
    bl fn_800FA280
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
