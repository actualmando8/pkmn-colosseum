    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r27, r3
    lwz r0, lbl_8047A424(r13)
    lis r3, lbl_803A2518@ha
    addi r30, r3, lbl_803A2518@l
    li r3, 0x0
    clrlwi r5, r0, 16
    li r4, 0x3
    bl fn_8012A5B0
    lwz r0, lbl_8047A420(r13)
    addi r3, r30, 0x170
    li r4, 0x3
    clrlwi r5, r0, 16
    bl fn_8012A5B0
    mr r31, r3
    li r28, 0x0
    li r29, 0x0
    li r3, 0x2
    li r4, 0x44d7
    li r5, 0x1
    li r6, 0x1
    bl fn_80106D3C
    lwz r5, lbl_8047A420(r13)
    mr r4, r31
    lwz r6, lbl_8047A40C(r13)
    li r3, 0x2
    bl fn_80089E20
    cmpwi r3, 0x0
    bne @L_8002DE2C
    li r3, 0x8
    li r4, 0x2
    li r5, 0x0
    bl fn_801D055C
    @L_8002DDB4:
    clrlwi r0, r29, 24
    cmplwi r0, 0x0
    bne @L_8002DDFC
    bl fn_801D04D0
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8002DDFC
    li r3, 0x2
    bl fn_80089D98
    cmpwi r3, 0x0
    blt @L_8002DDFC
    beq @L_8002DDF0
    li r3, 0x0
    bl fn_801D046C
    b @L_8002DDF8
    @L_8002DDF0:
    li r3, 0x1
    bl fn_801D046C
    @L_8002DDF8:
    li r29, 0x1
    @L_8002DDFC:
    bl fn_800F0308
    bl fn_801D04F4
    mr r31, r3
    cmpwi r31, 0x0
    beq @L_8002DDB4
    li r3, 0x2
    li r4, 0x1
    bl fn_8008ABE4
    bl fn_801D039C
    cmpwi r31, 0x4
    bne @L_8002DE2C
    li r28, 0x1
    @L_8002DE2C:
    clrlwi r0, r29, 24
    cmplwi r0, 0x0
    beq @L_8002DE44
    clrlwi r0, r28, 24
    cmplwi r0, 0x0
    bne @L_8002DEB4
    @L_8002DE44:
    li r3, 0x0
    li r4, 0x0
    bl fn_80129280
    li r0, 0x3bfa
    subi r5, r3, 0x4
    subi r4, r27, 0x4
    mtctr r0
    @L_8002DE60:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_8002DE60
    li r3, 0x2
    li r4, 0x44d6
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    bl fn_8001D7E4
    li r3, 0x1
    bl fn_801069FC
    addi r3, r30, 0xd18
    bl fn_8010A420
    addi r3, r30, 0xcd0
    bl fn_8010A420
    li r0, 0x0
    stb r0, lbl_804788B0(r13)
    stw r0, lbl_8047A42C(r13)
    b @L_8002DEFC
    @L_8002DEB4:
    li r3, 0x3c
    bl fn_800E0C04
    mr r28, r3
    b @L_8002DEC8
    @L_8002DEC4:
    bl fn_800F0308
    @L_8002DEC8:
    cmplwi r28, 0x0
    subi r28, r28, 0x1
    bne @L_8002DEC4
    li r3, 0x2
    li r4, 0x44d5
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    bl fn_8001D7E4
    li r3, 0x1
    bl fn_801069FC
    li r0, 0x13
    stw r0, lbl_8047A42C(r13)
    @L_8002DEFC:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
