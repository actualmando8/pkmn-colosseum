    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    lis r3, lbl_803A1C20@ha
    li r4, 0x0
    addi r3, r3, lbl_803A1C20@l
    li r5, 0x120
    bl memset
    lis r3, lbl_803A1D40@ha
    li r29, 0x0
    addi r30, r3, lbl_803A1D40@l
    lis r3, lbl_803A1C20@ha
    addi r31, r3, lbl_803A1C20@l
    b @L_8001D604
    @L_8001D3B4:
    clrlwi r4, r29, 16
    lwz r5, 0x8(r30)
    mulli r0, r4, 0x30
    lwz r3, 0xc(r30)
    cmpwi r5, 0x1
    li r27, 0x0
    add r28, r31, r0
    beq @L_8001D494
    bge @L_8001D3E4
    cmpwi r5, 0x0
    bge @L_8001D3F0
    b @L_8001D4FC
    @L_8001D3E4:
    cmpwi r5, 0x3
    bge @L_8001D4FC
    b @L_8001D4F0
    @L_8001D3F0:
    cmplwi r4, 0x6
    blt @L_8001D3FC
    b @L_8001D514
    @L_8001D3FC:
    cmpwi r5, 0x1
    beq @L_8001D43C
    bge @L_8001D470
    cmpwi r5, 0x0
    bge @L_8001D414
    b @L_8001D470
    @L_8001D414:
    li r3, 0x8ae
    bl fn_801906A0
    cmplwi r3, 0x0
    bne @L_8001D434
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    b @L_8001D474
    @L_8001D434:
    bl fn_8006AEEC
    b @L_8001D474
    @L_8001D43C:
    cmplwi r3, 0x0
    bne @L_8001D44C
    li r3, 0x0
    bl fn_801F2A7C
    @L_8001D44C:
    cmplwi r3, 0x0
    bne @L_8001D45C
    li r3, 0x0
    b @L_8001D474
    @L_8001D45C:
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    bl fn_801FB1C0
    b @L_8001D474
    @L_8001D470:
    li r3, 0x0
    @L_8001D474:
    cmplwi r3, 0x0
    bne @L_8001D484
    li r27, 0x0
    b @L_8001D514
    @L_8001D484:
    mr r4, r29
    bl fn_8012AC08
    mr r27, r3
    b @L_8001D4FC
    @L_8001D494:
    cmplwi r4, 0x6
    blt @L_8001D4A0
    b @L_8001D514
    @L_8001D4A0:
    cmplwi r3, 0x0
    bne @L_8001D4B0
    li r3, 0x0
    bl fn_801F2A7C
    @L_8001D4B0:
    cmplwi r3, 0x0
    bne @L_8001D4C0
    li r27, 0x0
    b @L_8001D514
    @L_8001D4C0:
    mr r4, r29
    bl fn_801F986C
    cmplwi r3, 0x0
    bne @L_8001D4D8
    li r27, 0x0
    b @L_8001D514
    @L_8001D4D8:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    mr r27, r3
    b @L_8001D4FC
    @L_8001D4F0:
    cmplwi r4, 0x1e
    blt @L_8001D4FC
    b @L_8001D514
    @L_8001D4FC:
    mr r3, r27
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8001D514
    li r27, 0x0
    @L_8001D514:
    cmplwi r27, 0x0
    bne @L_8001D528
    li r0, 0x0
    sth r0, 0x0(r28)
    b @L_8001D600
    @L_8001D528:
    mr r3, r27
    mr r4, r28
    bl fn_8011FDC8
    mr r3, r27
    li r4, 0x0
    li r5, 0x7b
    li r6, 0x0
    bl fn_8012640C
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8001D55C
    li r0, 0x0
    sth r0, 0x1a(r28)
    @L_8001D55C:
    mr r3, r27
    li r4, 0x0
    li r5, 0x7b
    li r6, 0x0
    bl fn_8012640C
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8001D584
    li r0, 0x1
    b @L_8001D5E8
    @L_8001D584:
    mr r3, r27
    bl fn_80120FE0
    clrlwi r0, r3, 16
    cmpwi r0, 0x3c
    beq @L_8001D5CC
    bge @L_8001D5AC
    cmpwi r0, 0x3a
    beq @L_8001D5BC
    bge @L_8001D5C4
    b @L_8001D5E4
    @L_8001D5AC:
    cmpwi r0, 0x3e
    beq @L_8001D5DC
    bge @L_8001D5E4
    b @L_8001D5D4
    @L_8001D5BC:
    li r0, 0x2
    b @L_8001D5E8
    @L_8001D5C4:
    li r0, 0x3
    b @L_8001D5E8
    @L_8001D5CC:
    li r0, 0x4
    b @L_8001D5E8
    @L_8001D5D4:
    li r0, 0x5
    b @L_8001D5E8
    @L_8001D5DC:
    li r0, 0x6
    b @L_8001D5E8
    @L_8001D5E4:
    li r0, 0x0
    @L_8001D5E8:
    clrlwi r0, r0, 16
    lis r3, lbl_802E4EB8@ha
    slwi r0, r0, 1
    addi r3, r3, lbl_802E4EB8@l
    lhzx r0, r3, r0
    sth r0, 0x24(r28)
    @L_8001D600:
    addi r29, r29, 0x1
    @L_8001D604:
    clrlwi r0, r29, 16
    cmplwi r0, 0x6
    blt @L_8001D3B4
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
