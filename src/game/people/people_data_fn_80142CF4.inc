    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    clrlwi. r0, r5, 16
    stw r31, 0xc(r1)
    mr r31, r6
    stw r30, 0x8(r1)
    mr r30, r5
    beq @L_80142D20
    cmplwi r0, 0x23
    blt @L_80142D28
    @L_80142D20:
    li r3, 0x0
    b @L_80142EE0
    @L_80142D28:
    cmplwi r0, 0xb
    bge @L_80142D48
    mr r3, r4
    bl fn_801440A0
    cmplwi r3, 0x0
    bne @L_80142D98
    li r3, 0x0
    b @L_80142EE0
    @L_80142D48:
    cmplwi r0, 0x18
    bge @L_80142D68
    mr r3, r4
    bl fn_80143DA0
    cmplwi r3, 0x0
    bne @L_80142D98
    li r3, 0x0
    b @L_80142EE0
    @L_80142D68:
    cmplwi r0, 0x1a
    bge @L_80142D88
    mr r3, r4
    bl fn_80143B48
    cmplwi r3, 0x0
    bne @L_80142D98
    li r3, 0x0
    b @L_80142EE0
    @L_80142D88:
    cmplwi r3, 0x0
    bne @L_80142D98
    li r3, 0x0
    b @L_80142EE0
    @L_80142D98:
    clrlwi r0, r30, 16
    cmplwi r0, 0x21
    bgt @L_80142EDC
    lis r4, jumptable_80367DE8@ha
    slwi r0, r0, 2
    addi r4, r4, jumptable_80367DE8@l
    lwzx r0, r4, r0
    mtctr r0
    bctr
    bl fn_80144088
    b @L_80142EE0
    bl fn_80144014
    clrlwi r3, r3, 24
    b @L_80142EE0
    bl fn_80143FFC
    clrlwi r3, r3, 16
    b @L_80142EE0
    bl fn_80143FCC
    clrlwi r3, r3, 24
    b @L_80142EE0
    bl fn_80143FB4
    clrlwi r3, r3, 24
    b @L_80142EE0
    bl fn_80143F84
    b @L_80142EE0
    bl fn_80143F6C
    clrlwi r3, r3, 16
    b @L_80142EE0
    bl fn_80143F54
    clrlwi r3, r3, 16
    b @L_80142EE0
    mr r4, r31
    bl fn_80143E2C
    extsb r3, r3
    b @L_80142EE0
    bl fn_80143E14
    b @L_80142EE0
    bl fn_80143D88
    clrlwi r3, r3, 16
    b @L_80142EE0
    bl fn_80143D70
    b @L_80142EE0
    bl fn_80143D58
    b @L_80142EE0
    bl fn_80143D40
    b @L_80142EE0
    bl fn_80143D28
    b @L_80142EE0
    bl fn_80143D10
    b @L_80142EE0
    bl fn_80143CF8
    b @L_80142EE0
    bl fn_80143CE0
    b @L_80142EE0
    bl fn_80143CC8
    b @L_80142EE0
    bl fn_80143CB0
    b @L_80142EE0
    bl fn_80143C98
    b @L_80142EE0
    bl fn_80143C80
    b @L_80142EE0
    bl fn_80143B30
    clrlwi r3, r3, 16
    b @L_80142EE0
    bl fn_80143C68
    clrlwi r3, r3, 16
    b @L_80142EE0
    bl fn_80143C50
    clrlwi r3, r3, 16
    b @L_80142EE0
    bl fn_8020A380
    clrlwi r3, r3, 16
    b @L_80142EE0
    bl fn_8020A368
    clrlwi r3, r3, 16
    b @L_80142EE0
    bl fn_8020A350
    b @L_80142EE0
    bl fn_8020A338
    b @L_80142EE0
    @L_80142EDC:
    li r3, 0x0
    @L_80142EE0:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
