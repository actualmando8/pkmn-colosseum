    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stw r31, 0x2c(r1)
    bl OSGetArenaLo
    bl OSGetArenaHi
    bl fn_8009F3D4
    lis r4, lbl_80465568@ha
    stw r3, lbl_80465568@l(r4)
    bl OSGetArenaLo
    mr r31, r3
    bl OSGetArenaHi
    lis r4, lbl_80465568@ha
    lis r5, lbl_80465568@ha
    addi r6, r4, lbl_80465568@l
    lwz r0, 0x0(r6)
    lis r4, lbl_80465568@ha
    addi r5, r5, lbl_80465568@l
    subf r0, r3, r0
    addi r4, r4, lbl_80465568@l
    add r6, r0, r31
    lwz r5, 0x8(r5)
    lwz r0, lbl_8047B288(r13)
    lis r3, lbl_80465568@ha
    lwz r4, 0xc(r4)
    subf r5, r5, r6
    cmpwi r0, 0x0
    addi r3, r3, lbl_80465568@l
    subf r0, r4, r5
    stw r0, 0x4(r3)
    bne @L_8019CB5C
    lis r7, fn_8019C8F4@ha
    lis r3, fn_8019C7B8@ha
    addi r0, r3, fn_8019C7B8@l
    lis r6, fn_8019C89C@ha
    addi r8, r7, fn_8019C8F4@l
    lis r5, fn_8019C854@ha
    addi r7, r6, fn_8019C89C@l
    lis r4, fn_8019C7E0@ha
    addi r6, r5, fn_8019C854@l
    stw r8, 0x8(r1)
    addi r5, r4, fn_8019C7E0@l
    addi r3, r1, 0x8
    stw r7, 0xc(r1)
    li r4, 0x14
    stw r6, 0x14(r1)
    stw r5, 0x10(r1)
    stw r0, 0x18(r1)
    bl fn_801A69C0
    lwz r0, lbl_8047B270(r13)
    cmplwi r0, 0x0
    beq @L_8019CABC
    lwz r0, lbl_8047B274(r13)
    cmplwi r0, 0x0
    beq @L_8019CABC
    lwz r5, lbl_8047B270(r13)
    li r4, 0x0
    lwz r3, lbl_8047B274(r13)
    li r0, 0x0
    stw r5, lbl_8047B268(r13)
    stw r3, lbl_8047B26C(r13)
    lwz r3, lbl_8047B268(r13)
    stw r4, lbl_8047B270(r13)
    lwz r4, lbl_8047B26C(r13)
    stw r0, lbl_8047B274(r13)
    bl fn_8009ABD0
    stw r3, lbl_80478C70(r13)
    lwz r3, lbl_80478C70(r13)
    bl fn_8009AB50
    lwz r5, lbl_8047B268(r13)
    lis r4, lbl_80465568@ha
    lwz r0, lbl_8047B26C(r13)
    lis r3, lbl_80465568@ha
    addi r4, r4, lbl_80465568@l
    subf r0, r5, r0
    addi r3, r3, lbl_80465568@l
    stw r0, 0x10(r4)
    li r4, 0x0
    lwz r3, 0x10(r3)
    bl fn_801AA538
    b @L_8019CB5C
    @L_8019CABC:
    bl OSGetArenaLo
    addi r0, r3, 0x1f
    clrrwi r31, r0, 5
    bl OSGetArenaHi
    lwz r0, lbl_8047B284(r13)
    stw r31, lbl_8047B268(r13)
    cmpwi r0, 0x0
    ble @L_8019CB08
    lwz r0, lbl_8047B284(r13)
    add r0, r31, r0
    stw r0, lbl_8047B26C(r13)
    lwz r0, lbl_8047B26C(r13)
    cmplw r0, r3
    ble @L_8019CAF8
    stw r3, lbl_8047B26C(r13)
    @L_8019CAF8:
    lwz r0, lbl_8047B26C(r13)
    clrrwi r0, r0, 5
    stw r0, lbl_8047B26C(r13)
    b @L_8019CB10
    @L_8019CB08:
    clrrwi r0, r3, 5
    stw r0, lbl_8047B26C(r13)
    @L_8019CB10:
    lwz r3, lbl_8047B268(r13)
    lwz r4, lbl_8047B26C(r13)
    bl fn_8009ABD0
    stw r3, lbl_80478C70(r13)
    lwz r3, lbl_80478C70(r13)
    bl fn_8009AB50
    lwz r5, lbl_8047B268(r13)
    lis r4, lbl_80465568@ha
    lwz r0, lbl_8047B26C(r13)
    lis r3, lbl_80465568@ha
    addi r4, r4, lbl_80465568@l
    subf r0, r5, r0
    addi r3, r3, lbl_80465568@l
    stw r0, 0x10(r4)
    li r4, 0x0
    lwz r3, 0x10(r3)
    bl fn_801AA538
    lwz r3, lbl_8047B26C(r13)
    bl OSSetArenaLo
    @L_8019CB5C:
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
