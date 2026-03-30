    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r29, r3
    mr r30, r5
    lbz r0, 0x2(r29)
    lwz r31, 0x1c(r29)
    extsb r0, r0
    cmpwi r0, 0x0
    beq @L_80070DB8
    li r3, 0x0
    b @L_80071088
    @L_80070DB8:
    lbz r0, 0x1(r29)
    extsb r0, r0
    cmpwi r0, 0x3
    beq @L_80070F28
    bge @L_80071084
    cmpwi r0, 0x0
    beq @L_80070DD8
    b @L_80071084
    @L_80070DD8:
    cmplwi r4, 0x0
    beq @L_80070ECC
    mr r27, r4
    li r26, 0x0
    lis r3, lbl_80267EA8@ha
    addi r28, r3, lbl_80267EA8@l
    b @L_80070E14
    @L_80070DF4:
    lwz r0, 0x4(r27)
    mr r3, r29
    lhz r4, 0x0(r27)
    slwi r0, r0, 2
    lhzx r5, r28, r0
    bl fn_801081F8
    addi r27, r27, 0x8
    addi r26, r26, 0x1
    @L_80070E14:
    cmplw r26, r30
    blt @L_80070DF4
    b @L_80070ECC
    @L_80070E20:
    lha r0, 0x50(r31)
    cmpwi r0, 0x12c
    bge @L_80070E58
    lha r0, 0x52(r31)
    cmpwi r0, 0x64
    bge @L_80070E40
    li r4, 0x3
    b @L_80070E80
    @L_80070E40:
    cmpwi r0, 0xc8
    bge @L_80070E50
    li r4, 0x4
    b @L_80070E80
    @L_80070E50:
    li r4, 0x5
    b @L_80070E80
    @L_80070E58:
    lha r0, 0x52(r31)
    cmpwi r0, 0x64
    bge @L_80070E6C
    li r4, 0x6
    b @L_80070E80
    @L_80070E6C:
    cmpwi r0, 0xc8
    bge @L_80070E7C
    li r4, 0x7
    b @L_80070E80
    @L_80070E7C:
    li r4, 0x8
    @L_80070E80:
    cmplwi r31, 0x0
    li r3, 0x0
    beq @L_80070EA8
    lwz r0, 0xc(r31)
    cmplwi r0, 0x0
    beq @L_80070EA8
    lbz r0, 0x46(r31)
    cmplwi r0, 0x0
    bne @L_80070EA8
    li r3, 0x1
    @L_80070EA8:
    cmpwi r3, 0x0
    bne @L_80070EC8
    lis r3, lbl_80267EA8@ha
    slwi r0, r4, 2
    addi r4, r3, lbl_80267EA8@l
    lhzx r4, r4, r0
    addi r3, r31, 0xc
    bl fn_80108518
    @L_80070EC8:
    lwz r31, 0x0(r31)
    @L_80070ECC:
    cmplwi r31, 0x0
    bne @L_80070E20
    lwz r27, 0x20(r29)
    b @L_80070F1C
    @L_80070EDC:
    cmplwi r27, 0x0
    li r3, 0x0
    beq @L_80070F04
    lwz r0, 0xc(r27)
    cmplwi r0, 0x0
    beq @L_80070F04
    lbz r0, 0x46(r27)
    cmplwi r0, 0x0
    bne @L_80070F04
    li r3, 0x1
    @L_80070F04:
    cmpwi r3, 0x0
    bne @L_80070F18
    addi r3, r27, 0xc
    li r4, 0x1ca
    bl fn_80108518
    @L_80070F18:
    lwz r27, 0x0(r27)
    @L_80070F1C:
    cmplwi r27, 0x0
    bne @L_80070EDC
    b @L_80071084
    @L_80070F28:
    cmplwi r4, 0x0
    beq @L_80071024
    mr r27, r4
    li r26, 0x0
    lis r3, lbl_80267EA8@ha
    addi r28, r3, lbl_80267EA8@l
    b @L_80070F68
    @L_80070F44:
    lwz r0, 0x4(r27)
    mr r3, r29
    lhz r4, 0x0(r27)
    slwi r0, r0, 2
    add r5, r28, r0
    lhz r5, 0x2(r5)
    bl fn_801081F8
    addi r27, r27, 0x8
    addi r26, r26, 0x1
    @L_80070F68:
    cmplw r26, r30
    blt @L_80070F44
    b @L_80071024
    @L_80070F74:
    lha r0, 0x50(r31)
    cmpwi r0, 0x12c
    bge @L_80070FAC
    lha r0, 0x52(r31)
    cmpwi r0, 0x64
    bge @L_80070F94
    li r4, 0x3
    b @L_80070FD4
    @L_80070F94:
    cmpwi r0, 0xc8
    bge @L_80070FA4
    li r4, 0x4
    b @L_80070FD4
    @L_80070FA4:
    li r4, 0x5
    b @L_80070FD4
    @L_80070FAC:
    lha r0, 0x52(r31)
    cmpwi r0, 0x64
    bge @L_80070FC0
    li r4, 0x6
    b @L_80070FD4
    @L_80070FC0:
    cmpwi r0, 0xc8
    bge @L_80070FD0
    li r4, 0x7
    b @L_80070FD4
    @L_80070FD0:
    li r4, 0x8
    @L_80070FD4:
    cmplwi r31, 0x0
    li r3, 0x0
    beq @L_80070FFC
    lwz r0, 0xc(r31)
    cmplwi r0, 0x0
    beq @L_80070FFC
    lbz r0, 0x46(r31)
    cmplwi r0, 0x0
    bne @L_80070FFC
    li r3, 0x1
    @L_80070FFC:
    cmpwi r3, 0x0
    bne @L_80071020
    lis r3, lbl_80267EA8@ha
    slwi r4, r4, 2
    addi r0, r3, lbl_80267EA8@l
    add r4, r0, r4
    addi r3, r31, 0xc
    lhz r4, 0x2(r4)
    bl fn_80108518
    @L_80071020:
    lwz r31, 0x0(r31)
    @L_80071024:
    cmplwi r31, 0x0
    bne @L_80070F74
    lwz r27, 0x20(r29)
    b @L_80071074
    @L_80071034:
    cmplwi r27, 0x0
    li r3, 0x0
    beq @L_8007105C
    lwz r0, 0xc(r27)
    cmplwi r0, 0x0
    beq @L_8007105C
    lbz r0, 0x46(r27)
    cmplwi r0, 0x0
    bne @L_8007105C
    li r3, 0x1
    @L_8007105C:
    cmpwi r3, 0x0
    bne @L_80071070
    addi r3, r27, 0xc
    li r4, 0x1ce
    bl fn_80108518
    @L_80071070:
    lwz r27, 0x0(r27)
    @L_80071074:
    cmplwi r27, 0x0
    bne @L_80071034
    li r0, 0x1
    stb r0, 0x2(r29)
    @L_80071084:
    li r3, 0x1
    @L_80071088:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
