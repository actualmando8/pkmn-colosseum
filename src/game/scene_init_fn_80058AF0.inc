    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r29, r3
    lwz r27, 0x60(r29)
    li r31, 0x1
    bl fn_800576B4
    cmpwi r3, 0x4
    beq @L_80058C68
    bge @L_80058B34
    cmpwi r3, 0x2
    beq @L_80058C38
    bge @L_80058C50
    cmpwi r3, 0x1
    bge @L_80058B44
    b @L_80058DB4
    @L_80058B34:
    cmpwi r3, 0x6
    beq @L_80058D30
    bge @L_80058DB4
    b @L_80058D08
    @L_80058B44:
    bl fn_80057538
    cmpwi r3, 0x0
    beq @L_80058DB4
    lbz r0, 0x95(r29)
    extsb r0, r0
    cmpwi r0, 0x0
    blt @L_80058B7C
    cmpwi r0, 0x7
    bge @L_80058B7C
    lis r3, lbl_802677D0@ha
    slwi r0, r0, 4
    addi r3, r3, lbl_802677D0@l
    add r3, r3, r0
    lwz r28, 0x8(r3)
    @L_80058B7C:
    clrlwi r5, r28, 16
    li r3, 0x0
    li r4, 0x3
    bl fn_8012A5B0
    bl fn_800574FC
    cmpwi r28, 0x0
    blt @L_80058C2C
    cmpwi r28, 0x6
    bge @L_80058C2C
    addi r29, r28, 0x1
    b @L_80058C08
    @L_80058BA8:
    clrlwi r5, r29, 16
    li r3, 0x0
    li r4, 0x3
    bl fn_8012A5B0
    mr r30, r3
    cmplwi r30, 0x0
    beq @L_80058C10
    subi r0, r29, 0x1
    li r3, 0x0
    clrlwi r5, r0, 16
    li r4, 0x3
    bl fn_8012A5B0
    cmplwi r3, 0x0
    beq @L_80058C10
    li r0, 0x27
    subi r5, r3, 0x4
    subi r4, r30, 0x4
    mtctr r0
    @L_80058BF0:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_80058BF0
    addi r29, r29, 0x1
    @L_80058C08:
    cmpwi r29, 0x6
    blt @L_80058BA8
    @L_80058C10:
    cmpwi r29, 0x6
    blt @L_80058C2C
    li r3, 0x0
    li r4, 0x3
    li r5, 0x5
    bl fn_8012A5B0
    bl fn_80124A60
    @L_80058C2C:
    li r3, 0x2
    bl fn_800576C4
    b @L_80058DB4
    @L_80058C38:
    bl fn_80057538
    cmpwi r3, 0x0
    beq @L_80058DB4
    li r3, 0x3
    bl fn_800576C4
    b @L_80058DB4
    @L_80058C50:
    lwz r0, 0x0(r27)
    cmpwi r0, 0x0
    beq @L_80058DB4
    li r3, 0x4
    bl fn_800576C4
    b @L_80058DB4
    @L_80058C68:
    bl fn_80057538
    cmpwi r3, 0x0
    beq @L_80058DB4
    bl fn_800574E0
    li r29, 0x0
    mr r30, r3
    mr r27, r29
    b @L_80058CBC
    @L_80058C88:
    clrlwi r5, r27, 16
    li r3, 0x0
    li r4, 0x3
    bl fn_8012A5B0
    mr r28, r3
    cmplwi r28, 0x0
    beq @L_80058CCC
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80058CB8
    li r29, 0x1
    @L_80058CB8:
    addi r27, r27, 0x1
    @L_80058CBC:
    cmpwi r27, 0x6
    bge @L_80058CCC
    cmpwi r29, 0x0
    beq @L_80058C88
    @L_80058CCC:
    cmpwi r29, 0x0
    beq @L_80058CF8
    li r0, 0x27
    subi r5, r28, 0x4
    subi r4, r30, 0x4
    mtctr r0
    @L_80058CE4:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_80058CE4
    @L_80058CF8:
    bl fn_800574A8
    li r3, 0x5
    bl fn_800576C4
    b @L_80058DB4
    @L_80058D08:
    bl fn_80057538
    cmpwi r3, 0x0
    beq @L_80058DB4
    li r3, 0x0
    bl fn_800576C4
    lwz r0, 0x0(r27)
    cmpwi r0, 0x0
    beq @L_80058DB4
    li r31, 0x0
    b @L_80058DB4
    @L_80058D30:
    bl fn_80057538
    cmpwi r3, 0x0
    beq @L_80058DB4
    lbz r0, 0x95(r29)
    extsb r0, r0
    cmpwi r0, 0x0
    blt @L_80058D68
    cmpwi r0, 0x7
    bge @L_80058D68
    lis r3, lbl_802677D0@ha
    slwi r0, r0, 4
    addi r3, r3, lbl_802677D0@l
    add r3, r3, r0
    lwz r28, 0x8(r3)
    @L_80058D68:
    bl fn_800574E0
    mr r30, r3
    clrlwi r5, r28, 16
    li r3, 0x0
    li r4, 0x3
    bl fn_8012A5B0
    li r0, 0x27
    subi r5, r3, 0x4
    subi r4, r30, 0x4
    mtctr r0
    @L_80058D90:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_80058D90
    bl fn_800574A8
    bl fn_80057400
    li r3, 0x3
    bl fn_800576C4
    @L_80058DB4:
    mr r3, r31
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
