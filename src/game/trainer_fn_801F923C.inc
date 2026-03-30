    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r5, 0x4d
    li r6, 0x0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr r30, r4
    mr r29, r3
    li r4, 0x0
    bl fn_801FB1C0
    cmplwi r3, 0x0
    beq @L_801F9270
    bl fn_801F1460
    @L_801F9270:
    li r31, 0x0
    b @L_801F9294
    @L_801F9278:
    mr r3, r29
    mr r6, r31
    li r4, 0x0
    li r5, 0x46
    bl fn_801FB1C0
    bl fn_80207760
    addi r31, r31, 0x1
    @L_801F9294:
    clrlwi r0, r31, 16
    cmplwi r0, 0x2
    blt @L_801F9278
    mr r3, r29
    li r4, 0x0
    li r5, 0x47
    li r6, 0x0
    bl fn_801FB1C0
    bl fn_80207760
    bl fn_801EF634
    clrlwi r0, r3, 16
    cmplwi r0, 0x1
    bne @L_801F92D0
    li r3, 0x0
    b @L_801F93E4
    @L_801F92D0:
    mr r3, r29
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    clrlwi r31, r3, 16
    li r3, 0x0
    mr r4, r31
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    clrlwi. r0, r3, 16
    beq @L_801F9314
    mr r3, r29
    mr r4, r30
    bl fn_802342CC
    b @L_801F93E0
    @L_801F9314:
    mr r4, r31
    li r3, 0x0
    li r5, 0x4
    li r6, 0x0
    bl fn_801FB1C0
    clrlwi r0, r3, 16
    cmplwi r0, 0x1
    bne @L_801F933C
    li r0, 0x0
    b @L_801F9358
    @L_801F933C:
    cmplwi r0, 0x2
    beq @L_801F934C
    cmplwi r0, 0x3
    bne @L_801F9354
    @L_801F934C:
    li r0, 0x1
    b @L_801F9358
    @L_801F9354:
    li r0, 0x2
    @L_801F9358:
    clrlwi. r0, r0, 24
    bne @L_801F9374
    mr r3, r29
    mr r4, r30
    li r5, 0x0
    bl fn_8026316C
    b @L_801F93E0
    @L_801F9374:
    mr r4, r31
    li r3, 0x0
    li r5, 0x4
    li r6, 0x0
    bl fn_801FB1C0
    clrlwi r0, r3, 16
    cmplwi r0, 0x1
    bne @L_801F939C
    li r0, 0x0
    b @L_801F93B8
    @L_801F939C:
    cmplwi r0, 0x2
    beq @L_801F93AC
    cmplwi r0, 0x3
    bne @L_801F93B4
    @L_801F93AC:
    li r0, 0x1
    b @L_801F93B8
    @L_801F93B4:
    li r0, 0x2
    @L_801F93B8:
    clrlwi r0, r0, 24
    cmplwi r0, 0x1
    bne @L_801F93E0
    mr r3, r29
    mr r4, r30
    bl fn_80262508
    clrlwi. r0, r3, 24
    bne @L_801F93E0
    li r3, 0x0
    b @L_801F93E4
    @L_801F93E0:
    li r3, 0x1
    @L_801F93E4:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
