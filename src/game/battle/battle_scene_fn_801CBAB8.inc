    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    li r28, 0x0
    mr r30, r28
    mr r31, r28
    b @L_801CBB80
@L_801CBAE4:
    cmpwi r30, 0x2
    beq @L_801CBB48
    bge @L_801CBAFC
    cmpwi r30, 0x0
    beq @L_801CBB0C
    b @L_801CBB80
@L_801CBAFC:
    cmpwi r30, 0x4
    beq @L_801CBB7C
    bge @L_801CBB80
    b @L_801CBB6C
@L_801CBB0C:
    li r3, 0x2
    li r4, 0x3c46
    li r5, 0x1
    li r6, 0x1
    bl fn_80106D3C
    bl fn_8001E184
    extsb r29, r3
    li r3, 0x1
    bl fn_801069FC
    cmpwi r29, 0x0
    beq @L_801CBB40
    li r31, 0x1
    b @L_801CBB80
@L_801CBB40:
    li r30, 0x2
    b @L_801CBB80
@L_801CBB48:
    li r3, 0x1
    bl fn_800889E4
    cmpwi r3, 0x0
    bne @L_801CBB64
    li r30, 0x3
    li r28, 0x1
    b @L_801CBB80
@L_801CBB64:
    li r30, 0x4
    b @L_801CBB80
@L_801CBB6C:
    li r3, 0x395
    bl fn_800FF58C
    li r30, 0x4
    b @L_801CBB80
@L_801CBB7C:
    li r31, 0x1
@L_801CBB80:
    cmpwi r31, 0x0
    beq @L_801CBAE4
    mr r3, r28
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
