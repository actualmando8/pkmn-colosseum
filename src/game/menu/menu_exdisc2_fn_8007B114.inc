    nofralloc
    stwu r1, -0x90(r1)
    mflr r0
    stw r0, 0x94(r1)
    stw r31, 0x8c(r1)
    lis r3, lbl_803FAEF8@ha
    li r4, 0x0
    addi r6, r3, lbl_803FAEF8@l
@L_8007B130:
    mr r5, r4
    li r3, 0x8
    li r0, 0x2
    mtctr r0
@L_8007B140:
    rlwinm r0, r5, 0, 31, 31
    cmplwi r0, 0x0
    beq @L_8007B15C
    rlwinm r0, r5, 31, 1, 31
    xoris r5, r0, 0xedb8
    xori r5, r5, 0x8320
    b @L_8007B160
@L_8007B15C:
    rlwinm r5, r5, 31, 1, 31
@L_8007B160:
    rlwinm r0, r5, 0, 31, 31
    cmplwi r0, 0x0
    beq @L_8007B17C
    rlwinm r0, r5, 31, 1, 31
    xoris r5, r0, 0xedb8
    xori r5, r5, 0x8320
    b @L_8007B180
@L_8007B17C:
    rlwinm r5, r5, 31, 1, 31
@L_8007B180:
    rlwinm r0, r5, 0, 31, 31
    cmplwi r0, 0x0
    beq @L_8007B19C
    rlwinm r0, r5, 31, 1, 31
    xoris r5, r0, 0xedb8
    xori r5, r5, 0x8320
    b @L_8007B1A0
@L_8007B19C:
    rlwinm r5, r5, 31, 1, 31
@L_8007B1A0:
    rlwinm r0, r5, 0, 31, 31
    cmplwi r0, 0x0
    beq @L_8007B1BC
    rlwinm r0, r5, 31, 1, 31
    xoris r5, r0, 0xedb8
    xori r5, r5, 0x8320
    b @L_8007B1C0
@L_8007B1BC:
    rlwinm r5, r5, 31, 1, 31
@L_8007B1C0:
    subi r3, r3, 0x3
    bdnz @L_8007B140
    stw r5, 0x0(r6)
    addi r6, r6, 0x4
    addi r4, r4, 0x1
    cmpwi r4, 0x100
    blt @L_8007B130
    li r0, 0x0
    stw r0, lbl_8047A64C(r13)
    bl OSGetTick
    lis r5, 0xaaab
    lis r4, lbl_802EE608@ha
    subi r0, r5, 0x5555
    lis r5, lbl_803FADF8@ha
    mulhwu r6, r0, r3
    addi r4, r4, lbl_802EE608@l
    addi r0, r5, lbl_803FADF8@l
    rlwinm r6, r6, 31, 1, 31
    mulli r5, r6, 0x3
    subf r5, r5, r3
    mr r3, r0
    rlwinm r0, r5, 2, 0, 29
    lwzx r4, r4, r0
    bl fn_800CA968
    lis r3, lbl_803FADF8@ha
    addi r4, r1, 0x44
    addi r3, r3, lbl_803FADF8@l
    bl fn_800A501C
    cmpwi r3, 0x0
    beq @L_8007B33C
    lwz r3, 0x78(r1)
    cmplwi r3, 0x0
    beq @L_8007B33C
    lwz r4, lbl_8047A648(r13)
    addi r0, r3, 0x5b
    rlwinm r0, r0, 0, 0, 26
    cmplwi r4, 0x0
    stw r0, lbl_8047A650(r13)
    beq @L_8007B264
    lwz r3, lbl_80478980(r13)
    bl fn_8009AAD4
@L_8007B264:
    lwz r3, lbl_80478980(r13)
    lwz r4, lbl_8047A650(r13)
    bl fn_8009A9D8
    cmplwi r3, 0x0
    stw r3, lbl_8047A648(r13)
    beq @L_8007B33C
    lwz r5, lbl_8047A650(r13)
    li r4, 0x0
    bl memset
    addi r3, r1, 0x44
    bl fn_800A50E4
    lwz r0, lbl_8047A64C(r13)
    cmpwi r0, 0x0
    bne @L_8007B33C
    lis r3, lbl_803FADF8@ha
    addi r4, r1, 0x8
    addi r3, r3, lbl_803FADF8@l
    bl fn_800A501C
    cmpwi r3, 0x0
    beq @L_8007B33C
    lwz r31, 0x3c(r1)
    cmplwi r31, 0x0
    beq @L_8007B33C
    addi r0, r31, 0x1f
    lwz r4, lbl_8047A648(r13)
    addi r3, r1, 0x8
    li r6, 0x0
    rlwinm r5, r0, 0, 0, 26
    li r7, 0x2
    bl fn_800A541C
    cmpwi r3, 0x0
    blt @L_8007B33C
    cmplw r3, r31
    blt @L_8007B33C
    addi r3, r1, 0x8
    bl fn_800A50E4
    lwz r4, lbl_8047A648(r13)
    mr r5, r31
    addi r3, r4, 0x34
    bl fn_800C8174
    lwz r3, lbl_8047A648(r13)
    li r4, 0x0
    li r5, 0x34
    bl memset
    addi r3, r31, 0x34
    lwz r4, lbl_8047A648(r13)
    mr r5, r3
    lwz r0, lbl_8047A650(r13)
    add r3, r4, r3
    li r4, 0x0
    subf r5, r5, r0
    bl memset
    li r0, 0x1
    stw r0, lbl_8047A64C(r13)
@L_8007B33C:
    lwz r0, 0x94(r1)
    lwz r31, 0x8c(r1)
    mtlr r0
    addi r1, r1, 0x90
    blr
